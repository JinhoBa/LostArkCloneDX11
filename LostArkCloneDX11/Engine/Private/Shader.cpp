#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{pDevice, pContext}
{
}

CShader::CShader(CShader& Prototype)
	:CComponent{ Prototype },
	m_pTect{ Prototype.m_pTect },
	m_pInputLayout{ Prototype.m_pInputLayout },
	m_pPass{ Prototype.m_pPass },
	m_pWorldMatrix{ Prototype.m_pWorldMatrix },
	m_pViewMatrix{ Prototype.m_pViewMatrix },
	m_pProjMatrix{ Prototype.m_pProjMatrix },
	m_pSRV{Prototype.m_pSRV}
{
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{

	if(FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, nullptr, 0, 0, m_pDevice, &m_pEffects, &m_pBlob)))
		return E_FAIL;

	m_pTect = m_pEffects->GetTechniqueByIndex(0);
	m_pPass = m_pTect->GetPassByIndex(0);
	D3DX11_PASS_DESC Pass_Desc = {};
	m_pPass->GetDesc(&Pass_Desc);

	m_pWorldMatrix = m_pEffects->GetVariableByName("g_WorldMatrix")->AsMatrix();
	m_pViewMatrix = m_pEffects->GetVariableByName("g_ViewMatrix")->AsMatrix();
	m_pProjMatrix = m_pEffects->GetVariableByName("g_ProjMatrix")->AsMatrix();
	m_pSRV = m_pEffects->GetVariableByName("g_Texture2D")->AsShaderResource();

	if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElement, Pass_Desc.pIAInputSignature, Pass_Desc.IAInputSignatureSize, &m_pInputLayout)))
		return E_FAIL;
	
	

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
    return S_OK;
}

void CShader::SetMatrix(_float4x4& WorldMatrix, _float4x4& ViewMatrix, _float4x4& ProjMatrix)
{
	m_pWorldMatrix->SetMatrix(reinterpret_cast<float*>(&WorldMatrix));
	m_pViewMatrix->SetMatrix(reinterpret_cast<float*>(&ViewMatrix));
	m_pProjMatrix->SetMatrix(reinterpret_cast<float*>(&ProjMatrix));
}

HRESULT CShader::SetResource(ID3D11ShaderResourceView* pSRV)
{
	if (FAILED(m_pSRV->SetResource(pSRV)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CShader::Apply()
{
	if (FAILED(m_pPass->Apply(0, m_pContext)))
		return E_FAIL;

	return S_OK;
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
}
