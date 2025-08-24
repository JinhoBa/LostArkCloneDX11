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
	const _uint Get_MaterialIndex() { return m_iMaterialIndex; }

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh);
	virtual HRESULT Initialize_Prototype(ifstream& stream);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint	m_iMaterialIndex = {};

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& stream);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END