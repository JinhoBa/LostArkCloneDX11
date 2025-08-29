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
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize_Prototype(MODEL eModelType, ifstream& stream);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Bind_BoneMatrices(const vector<class CBone*>& Bones, class CShader* pShader, const _char* pConstantName);

	HRESULT Save_To_Binary(MODEL eModelType, const aiMesh* pAIMesh, ofstream& stream);


private:
	_uint				m_iMaterialIndex = {};
	_uint				m_iNumBones = {};
	_char				m_szName[MAX_PATH] = {};

	_float4x4*			m_pBoneMatrices = { nullptr };

	vector<_int>		m_BoneIndices;
	vector<_float4x4>	m_OffsetMatrices;

private:
	HRESULT Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_For_Anim(const CModel* pModel, const aiMesh* pAIMesh);

	HRESULT Ready_VertexBuffer_For_NonAnim_Binary(_uint iNumFames, ifstream& stream);
	HRESULT Ready_VertexBuffer_For_Anim_Binary(_uint iNumFames, ifstream& stream);


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, ifstream& stream);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END