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
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const aiMesh* pAIMesh);
	virtual HRESULT Initialize_Prototype(MODEL eModelType, ifstream& stream);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Save_To_Binary(MODEL eModelType, const aiMesh* pAIMesh, ofstream& stream);


private:
	_uint	m_iMaterialIndex = {};
	_uint	m_iNumBones = {};

private:
	HRESULT Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh);
	HRESULT Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh);

	HRESULT Ready_VertexBuffer_For_NonAnim_Binary(_uint iNumFames, ifstream& stream);
	HRESULT Ready_VertexBuffer_For_Anim_Binary(_uint iNumFames, ifstream& stream);


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const aiMesh* pAIMesh);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, ifstream& stream);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END