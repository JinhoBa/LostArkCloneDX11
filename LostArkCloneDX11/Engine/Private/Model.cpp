#include "Model.h"

#include "Mesh.h"
#include "Material.h"
#include "GameInstance.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext}
{
}

CModel::CModel(CModel& Prototype)
    :CComponent{ Prototype },
    m_iNumMeshes{Prototype.m_iNumMeshes},
    m_Meshes{Prototype.m_Meshes},
    m_Materials{Prototype.m_Materials }
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pTexture : m_Materials)
        Safe_AddRef(pTexture);
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
    _uint iFlag = {};

    iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded |
        aiProcessPreset_TargetRealtime_Fast;

    m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    //string strModelFilePath = string(pModelFilePath);
    //auto last = strModelFilePath.find_last_of('/');

    //strModelFilePath = strModelFilePath.substr(0, last + 1);
    //m_strFolderPath = m_pGameInstance->Utf8ToWstring(strModelFilePath.c_str());

    if (nullptr == m_pAiScene)
    {
        MSG_BOX("Failed to ReadFile...");
        return E_FAIL;
    }

    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelFilePath)))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
    m_Meshes[iMeshIndex]->Bind_Resources();
    m_Meshes[iMeshIndex]->Render();

    return S_OK;
}

HRESULT CModel::Bind_Material(_uint iMeshIndex, CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex)
{
    if (m_iNumMeshes <= iMeshIndex)
        return E_FAIL;

    _uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

    return m_Materials[iMaterialIndex]->Bind_SRV(pShader, pConstantName, eTextureType, iTextureIndex);
}

HRESULT CModel::Ready_Meshes()
{
    m_iNumMeshes = m_pAiScene->mNumMeshes;
 
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAiScene->mMeshes[i]);
        if (nullptr == pMesh)
        {
            MSG_BOX("Failed to Ready Mesh");
            return E_FAIL;
        }
        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
    m_iNumMaterials = m_pAiScene->mNumMaterials;

    aiMaterial** ppMaterial = m_pAiScene->mMaterials;

    for (_uint i = 0; i < m_iNumMaterials; ++i)
    {
        CMaterials* pMaterial = CMaterials::Create(m_pDevice, m_pContext, ppMaterial[i], pModelFilePath);

        if (nullptr == pMaterial)
            return E_FAIL;

        m_Materials.push_back(pMaterial);
    }

    return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pModelFilePath)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CModel");
        return nullptr;
    }

    return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
    CComponent* pInstance = new CModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CModel");
        return nullptr;
    }

    return pInstance;
}

void CModel::Free()
{
    __super::Free();

    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);
    m_Meshes.clear();

    for (auto& Material : m_Materials)
        Safe_Release(Material);
    m_Materials.clear();

    m_Importer.FreeScene();
}
