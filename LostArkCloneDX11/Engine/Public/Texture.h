#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTexture);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint								m_iNumSRV = {};
	vector<ID3D11ShaderResourceView*>	m_pSRVs = {};

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, _uint iNumTexture);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END