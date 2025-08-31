#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	_int Get_BoneIndex(const _char* pBoneName) const;

	_bool IsAnimationFinished();

public:
	virtual HRESULT Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize_Prototype_Binary(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

	HRESULT Bind_Material(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex = 0, const _char* pValueConstanceName = nullptr);
	HRESULT Bind_BoneMatrices(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName);
	void	Play_Animation(_uint iAnimAnimIndex, _float fTimeDelta, _bool bLoop = false);


private:
	const aiScene* m_pAiScene = {};
	Assimp::Importer			m_Importer = {};

	MODEL						m_eModel = {};

	_uint						m_iNumMeshes = {};
	_uint						m_iNumMaterials = {};
	_uint						m_iNumBones = {};
	_uint						m_iNumAnimations = {};

	_wstring					m_strFolderPath = {};
	_float4x4					m_PreTransformMatrix = {};

	_int						m_iCurrentAnimIndex = { -1 };

	vector<class CMesh*>		m_Meshes;
	vector<class CMaterials*>	m_Materials;
	vector<class CBone*>		m_Bones;
	vector<class CAnimation*>	m_Animations;


private:
	HRESULT Ready_Meshes(MODEL eModel);
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(aiNode* pNode, _int iParentIndex);
	HRESULT Ready_Animations();

	HRESULT Save_Binary_Model(MODEL eModel, const _char* pModelFielPath, _fmatrix PreTransformMatrix);
	HRESULT Load_Binary_Model(MODEL eModel, const _char* pModelFielPath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	static CModel* Create_BinaryFile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END