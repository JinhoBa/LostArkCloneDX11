#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{pDevice, pContext}
{
}

CShader::CShader(CShader& Prototype)
	:CComponent{ Prototype },
	m_pEffects{ Prototype.m_pEffects },
	m_iNumPasses{Prototype.m_iNumPasses },
	m_InputLayouts{Prototype.m_InputLayouts }
{
	Safe_AddRef(m_pEffects);

	for (auto pInuptLayout : m_InputLayouts)
	{
		Safe_AddRef(pInuptLayout);
	}
}


HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	_uint iHlslFlag = {};

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	isHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG

	ID3DBlob* pBlob = {};

	if(FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffects, &pBlob)))
		return E_FAIL;

	if (nullptr == m_pEffects)
		return E_FAIL;
	
	ID3DX11EffectTechnique* pTechinque = m_pEffects->GetTechniqueByIndex(0);

	if (nullptr == pTechinque)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC Technique_Desc = {};

	if(FAILED(pTechinque->GetDesc(&Technique_Desc)))
		return E_FAIL;

	m_iNumPasses = Technique_Desc.Passes;

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		ID3DX11EffectPass* pPass = pTechinque->GetPassByIndex(i);

		if (nullptr == pPass)
			return E_FAIL;

		D3DX11_PASS_DESC Pass_Desc = {};

		if (FAILED(pPass->GetDesc(&Pass_Desc)))
			return E_FAIL;

		ID3D11InputLayout* pInputLayout = {};

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElement, Pass_Desc.pIAInputSignature, Pass_Desc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* ConstantName, const _float4x4* pMatrix )
{
	ID3DX11EffectVariable* pVariable = m_pEffects->GetVariableByName(ConstantName);

	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();

	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));
}

HRESULT CShader::Bind_WorldMatrix(const _float4x4* pWorldMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffects->GetVariableByName("g_WorldMatrix");

	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();

	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pWorldMatrix));
}

HRESULT CShader::Bind_Resource(const _char* ConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffects->GetVariableByName(ConstantName);

	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVVariable = pVariable->AsShaderResource();

	if (nullptr == pSRVVariable)
		return E_FAIL;

	return pSRVVariable->SetResource(pSRV);
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (m_iNumPasses <= iPassIndex)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return m_pEffects->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElement)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CShader");
		return nullptr;
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CComponent* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CShader");
		return nullptr;
	}

	return pInstance;
}

void CShader::Free()
{
    __super::Free();

	for (auto pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	m_InputLayouts.clear();

	Safe_Release(m_pEffects);
}
