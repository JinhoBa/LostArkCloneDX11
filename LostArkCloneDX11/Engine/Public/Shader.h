#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT	Bind_Position(const _char* ConstantName, const _float4* pVector);
	HRESULT	Bind_Matrix(const _char* ConstantName, const _float4x4* pMatrix);
	HRESULT Bind_WorldMatrix(const _float4x4* pWorldMatrix);
	HRESULT Bind_Resource(const _char* ConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_Value(const _char* ConstantName, _float4* pValue);
	HRESULT Begin(_uint iPassIndex);

private:
	_uint									m_iNumPasses = {};
	ID3DX11Effect*							m_pEffects = { nullptr };
	vector<ID3D11InputLayout*>				m_InputLayouts = {};


public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END