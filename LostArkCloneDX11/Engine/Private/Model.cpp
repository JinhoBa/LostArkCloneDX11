#include "Model.h"

#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext}
{
}

CModel::CModel(CModel& Prototype)
    :CComponent{ Prototype },
    m_iNumMeshes{Prototype.m_iNumMeshes},
    m_Meshes{Prototype.m_Meshes}
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
    _uint iFlag = {};

    iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded |
        aiProcessPreset_TargetRealtime_Fast;

    m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    if (nullptr == m_pAiScene)
    {
        MSG_BOX("Failed to ReadFile...");
        return E_FAIL;
    }

    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render()
{
    for (auto& pMesh : m_Meshes)
    {
        pMesh->Bind_Resources();
        pMesh->Render();
    }
    return S_OK;
}

HRESULT CModel::Render_Mesh(_uint iIndex)
{
    if (m_iNumMeshes <= iIndex)
        return E_FAIL;


    m_Meshes[iIndex]->Bind_Resources();
    m_Meshes[iIndex]->Render();

    return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
    m_iNumMeshes = m_pAiScene->mNumMeshes;
 
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAiScene->mMeshes[i], m_pAiScene->mMaterials[i]);
        if (nullptr == pMesh)
        {
            MSG_BOX("Failed to Ready Mesh");
            return E_FAIL;
        }
        m_Meshes.push_back(pMesh);
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

    m_Importer.FreeScene();
}
