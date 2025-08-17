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
	virtual HRESULT Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(class CShader* pShader);
	HRESULT Render_Mesh(_uint iIndex);

private:
	const aiScene*			m_pAiScene = {};
	Assimp::Importer		m_Importer = {};

	_uint					m_iNumMeshes = {};
	_wstring				m_strFolderPath = {};
	vector<class CMesh*>	m_Meshes;
	vector<class CTexture*>	m_Textures;

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Textures();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END