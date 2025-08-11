#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTextureMap final : public CComponent
{
private:
	CTextureMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTextureMap(CTextureMap& Prototype);
	virtual ~CTextureMap() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pFilePath, vector<_wstring>* pFileNames);
	virtual HRESULT Initialize(void* pArg) override;

	ID3D11ShaderResourceView* Get_SRV(const _tchar* pFileName);

private:
	_uint										m_iNumSRV = {};
	map<_wstring, ID3D11ShaderResourceView*>	m_SRVs = {};

public:
	static CTextureMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const _tchar* pFilePath, vector<_wstring>* pFileNames);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END