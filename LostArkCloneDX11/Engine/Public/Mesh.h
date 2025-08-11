#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMesh :public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, const aiMaterial* pMaterials);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint m_iNumMaterial = {};

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, const aiMaterial* pMaterials);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END