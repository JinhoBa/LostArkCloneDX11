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

HRESULT CModel::Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, MODELFILE eModeFile, _fmatrix PreTransformMatrix)
{
    if (MODELFILE::FBX == eModeFile)
    {
        _uint iFlag = {};

        iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_GlobalScale;

        if (MODEL::NONANIM == eModel)
            iFlag |= aiProcess_PreTransformVertices;

        m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

        XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);


        if (nullptr == m_pAiScene)
        {
            MSG_BOX("Failed to ReadFile...");
            return E_FAIL;
        }

        if (FAILED(Ready_Meshes()))
            return E_FAIL;

        if (FAILED(Ready_Materials(pModelFilePath)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(Load_Binary_Model(pModelFilePath)))
            return E_FAIL;
    }
    


    return S_OK;
}

HRESULT CModel::Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    _uint iFlag = {};

    iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_GlobalScale;

    if (MODEL::NONANIM == eModel)
        iFlag |= aiProcess_PreTransformVertices;

    m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    if (nullptr == m_pAiScene)
    {
        MSG_BOX("Failed to ReadFile...");
        return E_FAIL;
    }

    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelFilePath)))
        return E_FAIL;

    _char szDrive[MAX_PATH] = {};
    _char szDir[MAX_PATH] = {};
    _char szFileName[MAX_PATH] = {};
    _char szExt[MAX_PATH] = {};
    
    _splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, nullptr, 0);

    _char szBinaryFilePath[MAX_PATH] = {};

    strcpy_s(szBinaryFilePath, szDrive);
    strcat_s(szBinaryFilePath, szDir);
    strcat_s(szBinaryFilePath, szFileName);
    strcat_s(szBinaryFilePath, ".bin");

    if (FAILED(Save_Binary_Model(szBinaryFilePath)))
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

HRESULT CModel::Bind_Material(_uint iMeshIndex, CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex, const _char* pValueConstanceName)
{
    if (m_iNumMeshes <= iMeshIndex)
        return E_FAIL;

    _uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

    if (FAILED(m_Materials[iMaterialIndex]->Bind_SRV(pShader, pConstantName, eTextureType, iTextureIndex)))
        return E_FAIL;
   

    if(nullptr != pValueConstanceName)
    {
        if (FAILED(m_Materials[iMaterialIndex]->Bind_Value(pShader, pValueConstanceName, eTextureType, iTextureIndex)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CModel::Save_Binary_Model(const _char* pModelFielPath)
{
    ofstream out(pModelFielPath, ios::binary);

    if (false == out.is_open())
    {
        MSG_BOX("Failed to Save Model Binanry File");
        return E_FAIL;
    }

    out.write(reinterpret_cast<const _char*>(&m_pAiScene->mNumMeshes), sizeof(_uint));

    /* Mesh */
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        aiMesh* Mesh = m_pAiScene->mMeshes[i];

        out.write(reinterpret_cast<const _char*>(&Mesh->mMaterialIndex), sizeof(_uint));
        out.write(reinterpret_cast<const _char*>(&Mesh->mNumVertices), sizeof(_uint));
        out.write(reinterpret_cast<const _char*>(&Mesh->mNumFaces), sizeof(_uint));

        _uint iNumVertices = Mesh->mNumVertices;
      
        for (_uint j = 0; j < iNumVertices; ++j)
        {
            out.write(reinterpret_cast<const _char*>(&Mesh->mVertices[j]), sizeof(_float3));
            out.write(reinterpret_cast<const _char*>(&Mesh->mNormals[j]), sizeof(_float3));
            out.write(reinterpret_cast<const _char*>(&Mesh->mTangents[j]), sizeof(_float3));
            out.write(reinterpret_cast<const _char*>(&Mesh->mTextureCoords[0][j]), sizeof(_float2));
        }
        
        _uint iNumFaces = Mesh->mNumFaces;
        
        for (_uint j = 0; j < iNumFaces; ++j)
        {
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[0]), sizeof(_uint));
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[1]), sizeof(_uint));
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[2]), sizeof(_uint));
        }
    }

    /* Material */
    aiMaterial** ppMaterial = m_pAiScene->mMaterials;

    out.write(reinterpret_cast<const _char*>(&m_pAiScene->mNumMaterials), sizeof(_uint));

    for (_uint i = 0; i < m_iNumMaterials; ++i)
    {
        aiMaterial* Material = ppMaterial[i];

        out.write(reinterpret_cast<const _char*>(Material->GetName().data), MAX_PATH);
    }

    out.close();

    return S_OK;
}

HRESULT CModel::Load_Binary_Model(const _char* pModelFielPath)
{
    ifstream in(pModelFielPath, ios::binary);

    if (false == in.is_open())
    {
        MSG_BOX("Failed to Load Model Binanry File");
        return E_FAIL;
    }

    /* Mesh */

    in.read(reinterpret_cast<_char*>(&m_iNumMeshes), sizeof(_uint));
    
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, in);
        if (nullptr == pMesh)
        {
            MSG_BOX("Failed to Ready Mesh from BinaryFile");
            return E_FAIL;
        }
        m_Meshes.push_back(pMesh);
    }

    in.read(reinterpret_cast<_char*>(&m_iNumMaterials), sizeof(_uint));

    for (_uint i = 0; i < m_iNumMaterials; ++i)
    {
        _char szMatrialFilePath[MAX_PATH] = {};
        in.read(reinterpret_cast<_char*>(szMatrialFilePath), MAX_PATH);

        CMaterials* pMaterial = CMaterials::Create(m_pDevice, m_pContext, szMatrialFilePath, pModelFielPath);

        if (nullptr == pMaterial)
            return E_FAIL;

        m_Materials.push_back(pMaterial);
    }
   
    in.close();


    return S_OK;
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

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, MODELFILE eModeFile, _fmatrix PreTransformMatrix)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eModel, pModelFilePath, eModeFile, PreTransformMatrix)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CModel");
        return nullptr;
    }

    return pInstance;
}

CModel* CModel::Create_BinaryFile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eModel, pModelFilePath, PreTransformMatrix)))
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
