#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	typedef struct Shader_Desc
	{
		const _char* szPassName = {};
	}SHADER_DESC;
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(CShader& Prototype);
	virtual ~CShader() = default;

public:
	ID3D11InputLayout* Get_Layout() { return m_pInputLayout; }
	HRESULT Set_Pass();
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Set_WorldMatrix(_float4x4& WorldMatrix);
	void Set_ViewMatrix(_float4x4& ViewMatrix);
	void Set_ProjMatrix(_float4x4& ProjMatrix);
	void Set_Matrix(_float4x4& WorldMatrix, _float4x4& ViewMatrix, _float4x4& ProjMatrix);
	HRESULT SetResource(ID3D11ShaderResourceView* pSRV);
	HRESULT Apply();


private:
	_uint									m_iNumElement = {};
	D3D11_INPUT_ELEMENT_DESC*				m_pElement_Desc = {};

	ID3DX11Effect*							m_pEffects = { nullptr };
	ID3DBlob*								m_pBlob = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };
	ID3DX11EffectTechnique*					m_pTect = { nullptr };
	ID3DX11EffectPass*						m_pPass = { nullptr };

	ID3DX11EffectMatrixVariable*			m_pWorldMatrix = { nullptr };
	ID3DX11EffectMatrixVariable*			m_pViewMatrix = { nullptr };
	ID3DX11EffectMatrixVariable*			m_pProjMatrix = { nullptr };
	ID3DX11EffectShaderResourceVariable*	m_pSRV = { nullptr };

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END