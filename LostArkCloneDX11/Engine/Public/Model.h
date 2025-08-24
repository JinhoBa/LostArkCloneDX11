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
	_uint Get_NumMeshes() { return m_iNumMeshes; }

public:
	virtual HRESULT Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, MODELFILE eModeFile, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);
	
	HRESULT Bind_Material(_uint iMeshIndex, class CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex = 0, const _char* pValueConstanceName = nullptr);

	HRESULT Save_Binary_Model(const _char* pModelFielPath);
	HRESULT Load_Binary_Model(const _char* pModelFielPath);

private:

	const aiScene*				m_pAiScene = {};
	Assimp::Importer			m_Importer = {};

	MODEL						m_eModel = {};

	_uint						m_iNumMeshes = {};
	_uint						m_iNumMaterials = {};

	vector<class CMesh*>		m_Meshes;
	vector<class CMaterials*>	m_Materials;

	_wstring					m_strFolderPath = {};

	_float4x4					m_PreTransformMatrix = {};

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const _char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, MODELFILE eModeFile = MODELFILE::FBX,_fmatrix PreTransformMatrix = XMMatrixIdentity());
	static CModel* Create_BinaryFile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END