#include "Model.h"

#include "GameInstance.h"

#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext }
{
}

CModel::CModel(CModel& Prototype)
    :CComponent{ Prototype },
    m_eModel{ Prototype.m_eModel },
    m_iNumMeshes{ Prototype.m_iNumMeshes },
    m_iNumMaterials{ Prototype.m_iNumMaterials },
    m_iNumAnimations{ Prototype.m_iNumAnimations },
    m_Meshes{ Prototype.m_Meshes },
    m_Materials{ Prototype.m_Materials },
    m_Bones{ Prototype.m_Bones },
    m_Animations{ Prototype.m_Animations },
    m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
{
    for (auto& pBone : m_Bones)
        Safe_AddRef(pBone);

    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);

    for (auto& pAnimaiton : m_Animations)
        Safe_AddRef(pAnimaiton);
}

_int CModel::Get_BoneIndex(const _char* pBoneName) const
{
    _int	iBoneIndex = {};

    auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
        {
            if (true == pBone->Compare_Name(pBoneName))
                return true;

            ++iBoneIndex;

            return false;
        });

    if (iter == m_Bones.end())
        return -1;

    return iBoneIndex;
}

_bool CModel::IsAnimationFinished()
{
    if (-1 == m_iCurrentAnimIndex || (_int)m_iNumAnimations <= m_iCurrentAnimIndex)
        return false;

    return m_Animations[m_iCurrentAnimIndex]->IsAnimationFinished();
}

HRESULT CModel::Initialize_Prototype(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    _char szExt[MAX_PATH] = {};

    m_eModel = eModel;

    _splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

    if (!strcmp(szExt, ".fbx"))
    {
        _uint			iFlag = {};

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


        Ready_Bones(m_pAiScene->mRootNode, -1);

        if (FAILED(Ready_Meshes(m_eModel)))
            return E_FAIL;

        if (FAILED(Ready_Materials(pModelFilePath)))
            return E_FAIL;

        if (FAILED(Ready_Animations()))
            return E_FAIL;

    }
    else
    {
        XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

        if (FAILED(Load_Binary_Model(eModel, pModelFilePath)))
            return E_FAIL;
    }


    return S_OK;
}

HRESULT CModel::Initialize_Prototype_Binary(MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    _uint iFlag = {};

    iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_GlobalScale;

    if (MODEL::NONANIM == eModel)
        iFlag |= aiProcess_PreTransformVertices;

    m_pAiScene = m_Importer.ReadFile(pModelFilePath, iFlag);

    m_eModel = eModel;

    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    if (nullptr == m_pAiScene)
    {
        MSG_BOX("Failed to ReadFile...");
        return E_FAIL;
    }

    Ready_Bones(m_pAiScene->mRootNode, -1);

    if (FAILED(Ready_Meshes(m_eModel)))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelFilePath)))
        return E_FAIL;

    if (FAILED(Ready_Animations()))
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

    if (FAILED(Save_Binary_Model(eModel, szBinaryFilePath, PreTransformMatrix)))
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


    if (nullptr != pValueConstanceName)
    {
        if (FAILED(m_Materials[iMaterialIndex]->Bind_Value(pShader, pValueConstanceName, eTextureType, iTextureIndex)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CModel::Bind_BoneMatrices(_uint iMeshIndex, CShader* pShader, const _char* pConstantName)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    return m_Meshes[iMeshIndex]->Bind_BoneMatrices(m_Bones, pShader, pConstantName);
}

void CModel::Play_Animation(_uint iAnimAnimIndex, _float fTimeDelta, _bool bLoop)
{
    if (m_iCurrentAnimIndex != iAnimAnimIndex && -1 != m_iCurrentAnimIndex)
    {
        m_Animations[m_iCurrentAnimIndex]->Reset_TrackPosition();
    }

    m_iCurrentAnimIndex = iAnimAnimIndex;

    if (-1 == m_iCurrentAnimIndex || (_int)m_iNumAnimations <= m_iCurrentAnimIndex)
        return;

    m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrix(m_Bones, fTimeDelta);

    if (bLoop)
    {
        if(true == m_Animations[m_iCurrentAnimIndex]->IsAnimationFinished())
            m_Animations[m_iCurrentAnimIndex]->Reset_TrackPosition();
    }

    for (auto& pBone : m_Bones)
    {
        pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
    }
}

HRESULT CModel::Ready_Meshes(MODEL eModel)
{
    m_iNumMeshes = m_pAiScene->mNumMeshes;

    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModel, this, m_pAiScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
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

HRESULT CModel::Ready_Bones(aiNode* pAINode, _int iParentIndex)
{
    CBone* pBone = CBone::Create(pAINode, iParentIndex);
    if (nullptr == pBone)
        return E_FAIL;

    m_Bones.push_back(pBone);

    _int	iParent = (_int)m_Bones.size() - 1;

    for (size_t i = 0; i < pAINode->mNumChildren; i++)
    {
        Ready_Bones(pAINode->mChildren[i], iParent);
    }


    return S_OK;
}

HRESULT CModel::Ready_Animations()
{
    m_iNumAnimations = m_pAiScene->mNumAnimations;

    for (_uint i = 0; i < m_iNumAnimations; i++)
    {
        CAnimation* pAnimation = CAnimation::Create(this, m_pAiScene->mAnimations[i]);

        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

HRESULT CModel::Save_Binary_Model(MODEL eModel, const _char* pModelFielPath, _fmatrix PreTransformMatrix)
{
    ofstream out(pModelFielPath, ios::binary);

    if (false == out.is_open())
    {
        MSG_BOX("Failed to Save Model Binanry File");
        return E_FAIL;
    }

#pragma region BONES
    m_iNumBones = (_uint)m_Bones.size();
    out.write(reinterpret_cast<const _char*>(&m_iNumBones), sizeof(_uint));

    for (auto& pBone : m_Bones)
    {
        pBone->Save_To_Binary(out, XMLoadFloat4x4(&m_PreTransformMatrix));
    }
#pragma endregion
    
#pragma region MESH
    out.write(reinterpret_cast<const _char*>(&m_pAiScene->mNumMeshes), sizeof(_uint));

    /* Mesh */
    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        aiMesh* Mesh = m_pAiScene->mMeshes[i];

        out.write(reinterpret_cast<const _char*>(&Mesh->mMaterialIndex), sizeof(_uint));
        out.write(reinterpret_cast<const _char*>(&Mesh->mNumVertices), sizeof(_uint));
        out.write(reinterpret_cast<const _char*>(&Mesh->mNumFaces), sizeof(_uint));

        if (FAILED(m_Meshes[i]->Save_To_Binary(eModel, Mesh, out)))
            return E_FAIL;

        _uint iNumFaces = Mesh->mNumFaces;

        for (_uint j = 0; j < iNumFaces; ++j)
        {
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[0]), sizeof(_uint));
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[1]), sizeof(_uint));
            out.write(reinterpret_cast<const _char*>(&Mesh->mFaces[j].mIndices[2]), sizeof(_uint));
        }
    }
#pragma endregion
    
#pragma region MATERIAL
    /* Material */
    aiMaterial** ppMaterial = m_pAiScene->mMaterials;

    out.write(reinterpret_cast<const _char*>(&m_pAiScene->mNumMaterials), sizeof(_uint));

    for (_uint i = 0; i < m_iNumMaterials; ++i)
    {
        aiMaterial* Material = ppMaterial[i];

        out.write(reinterpret_cast<const _char*>(Material->GetName().data), MAX_PATH);
    }
#pragma endregion

#pragma region ANMATION
    out.write(reinterpret_cast<const _char*>(&m_iNumAnimations), sizeof(_uint));

    for (auto& pAnimation : m_Animations)
    {
        pAnimation->Save_To_Binary(out);
    }
#pragma endregion

    out.close();

    return S_OK;
}

HRESULT CModel::Load_Binary_Model(MODEL eModel, const _char* pModelFielPath)
{
    ifstream in(pModelFielPath, ios::binary);

    if (false == in.is_open())
    {
        MSG_BOX("Failed to Load Model Binanry File");
        return E_FAIL;
    }

#pragma region READY_BONES

    in.read(reinterpret_cast<_char*>(&m_iNumBones), sizeof(_uint));
    
    m_Bones.reserve((size_t)m_iNumBones);

    for (_uint i = 0; i < m_iNumBones; ++i)
    {
        CBone* pBone = CBone::Create(in);

        if (nullptr == pBone)
            return E_FAIL;

        m_Bones.push_back(pBone);
    }
    
#pragma endregion

#pragma region READY_MESH

    in.read(reinterpret_cast<_char*>(&m_iNumMeshes), sizeof(_uint));

    m_Meshes.reserve((size_t)m_iNumMeshes);

    for (_uint i = 0; i < m_iNumMeshes; ++i)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, eModel, in);
        if (nullptr == pMesh)
        {
            MSG_BOX("Failed to Ready Mesh from BinaryFile");
            return E_FAIL;
        }
        m_Meshes.push_back(pMesh);
    }

#pragma endregion

#pragma region READY_MATERIAL

    in.read(reinterpret_cast<_char*>(&m_iNumMaterials), sizeof(_uint));

    m_Materials.reserve((size_t)m_iNumMaterials);

    for (_uint i = 0; i < m_iNumMaterials; ++i)
    {
        _char szMatrialFilePath[MAX_PATH] = {};
        in.read(reinterpret_cast<_char*>(szMatrialFilePath), MAX_PATH);

        CMaterials* pMaterial = CMaterials::Create(m_pDevice, m_pContext, szMatrialFilePath, pModelFielPath);

        if (nullptr == pMaterial)
            return E_FAIL;

        m_Materials.push_back(pMaterial);
    }

#pragma endregion

#pragma region READY_ANMATION

    in.read(reinterpret_cast<_char*>(&m_iNumAnimations), sizeof(_uint));

    m_Animations.reserve((size_t)m_iNumAnimations);

    for (_uint i = 0; i < m_iNumAnimations; ++i)
    {
        CAnimation* pAnimation = CAnimation::Create(in);

        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

#pragma endregion

    in.close();


    return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
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

CModel* CModel::Create_BinaryFile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModel, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype_Binary(eModel, pModelFilePath, PreTransformMatrix)))
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

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();

    for (auto& pBone : m_Bones)
        Safe_Release(pBone);
    m_Bones.clear();

    for (auto& pAnimation : m_Animations)
        Safe_Release(pAnimation);
    m_Animations.clear();

    m_Importer.FreeScene();
}
