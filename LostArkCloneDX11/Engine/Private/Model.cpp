#include "Model.h"

#include "Mesh.h"
#include "Texture.h"
#include "GameInstance.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext}
{
}

CModel::CModel(CModel& Prototype)
    :CComponent{ Prototype },
    m_iNumMeshes{Prototype.m_iNumMeshes},
    m_Meshes{Prototype.m_Meshes},
    m_Textures{Prototype.m_Textures }
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pTexture : m_Textures)
        Safe_AddRef(pTexture);
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
    _uint iFlag = {};

    iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded |
        aiProcessPreset_TargetRealtime_Fast;

    m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    string strModelFilePath = string(pModelFilePath);
   auto last = strModelFilePath.find_last_of('/');

   strModelFilePath = strModelFilePath.substr(0, last + 1);
   m_strFolderPath = m_pGameInstance->Utf8ToWstring(strModelFilePath.c_str());

    if (nullptr == m_pAiScene)
    {
        MSG_BOX("Failed to ReadFile...");
        return E_FAIL;
    }

    if (FAILED(Ready_Meshes()))
        return E_FAIL;

 if (FAILED(Ready_Textures()))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render(CShader* pShader)
{
    for (_uint i = 0; i< m_iNumMeshes; ++i)
    {
        if (FAILED(pShader->Bind_Resource("g_Texture2D", m_Textures[i]->Get_SRV(0))))
            return E_FAIL;

        if (FAILED(pShader->Begin(0)))
            return E_FAIL;

        m_Meshes[i]->Bind_Resources();
        m_Meshes[i]->Render();
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

HRESULT CModel::Ready_Textures()
{
    aiMaterial** ppMaterial = m_pAiScene->mMaterials;
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        aiString strTexture = {};
        aiMaterial* pMaterial = ppMaterial[i];
        pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &strTexture);

        _wstring strFilePath = CGameInstance::GetInstance()->Utf8ToWstring(strTexture.C_Str());

        auto iIndex = strFilePath.find_last_of('\\');

        _wstring strFileName = strFilePath.substr(iIndex + 1);

        strFilePath = m_strFolderPath + strFileName;


        CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, strFilePath.c_str(), 1);
        if (nullptr == pTexture)
        {
            MSG_BOX("Failed to Ready Texture");
            return E_FAIL;
        }
        m_Textures.push_back(pTexture);
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

    for (auto& pTexture : m_Textures)
        Safe_Release(pTexture);
    m_Textures.clear();

    m_Importer.FreeScene();
}
