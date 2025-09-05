#include "pch.h"
#include "Body_Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    :CPartObject{ Prototype }
{

}

const _float4x4* CBody_Player::Get_BoneMatrixPtr(const _char* pBoneName) const
{
    return m_pModelCom->Get_BoneMatrixPrt(pBoneName);
}

HRESULT CBody_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_iAnimIndex = 35;

    m_pCamera = dynamic_cast<CCamera_Fix*>(CGameManager::GetInstance()->Get_Camera());

    if (nullptr == m_pCamera)
        return E_FAIL;

    m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, 35, true);

    m_iCameraTargetBoneIndex = m_pModelCom->Get_BoneIndex("b_cameratarget");

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

void CBody_Player::Update(_float fTimeDelta)
{
    m_isAnimationFinish = m_pModelCom->Play_Animation(fTimeDelta);
   
    /* 부모 행렬 적용 */
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));

    /* 카메라 타겟 설정 */
    m_pCamera->Set_CameraTargetPosition(
        XMVector3TransformCoord(
            m_pModelCom->Get_BoneMatirx(m_iCameraTargetBoneIndex).r[3], 
            XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix())));
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Player::Render()
{
#pragma region ANIMATION_TEST
    ImGui::InputInt("Animation", &m_iAnimIndex);
    _int iIndex = {};
    for (auto pName : m_pModelCom->Get_AnimationNames())
    {
        if (ImGui::Button(to_string(iIndex).c_str()))
        {
            m_iAnimIndex = iIndex;
            m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, m_iAnimIndex, true);
        }
        ++iIndex;
        ImGui::SameLine();
        ImGui::Text(pName);
    }
#pragma endregion

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pParentTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;


        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;


        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

void CBody_Player::Set_Animation(_uint iAnimationIdex, _bool bLoop, _float fLerpTime)
{
    m_isAnimationFinish = false;

    m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, iAnimationIdex, bLoop, fLerpTime);
}

HRESULT CBody_Player::Add_Components()
{
    /* AnimModel */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Shader_VTXAnimTex */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CBody_Player");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CGameObject* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CBody_Player");
        return nullptr;
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
