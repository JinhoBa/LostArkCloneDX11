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

_float CBody_Player::Get_TrackPoisiton()
{
    return m_pModelCom->Get_TrackPosition();
}
void CBody_Player::Toggle_RimLight()
{
    m_bApplyRimLight = true;

    m_fRimStrength = 1.f;
}
HRESULT CBody_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    BODYPLAYER_DESC* pDesc = static_cast<BODYPLAYER_DESC*>(pArg);

    m_pAttackSpeed = pDesc->pAttackSpeed;
    m_pStance = pDesc->pStance;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Bind_CameraBoneMatrix()))
        return E_FAIL;

    m_bApplyRimLight = false;
    m_iAnimIndex = 35;

    m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, 35, true);

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pCameraTargetBoneMatrix = m_pModelCom->Get_BoneMatrixPrt("b_cameratarget");

    m_fRimStrength = 0.f;
    m_fRimPower = 1.f;

    m_vRimColor[ENUM_TO_INT(STANCE::FLURRY)] = _float4(0.4f, 0.8f, 1.f, 1.f);
    m_vRimColor[ENUM_TO_INT(STANCE::FOCUS)] = _float4(0.95f, 0.32f, 0.32f, 1.f);

#ifdef _DEBUG
    m_isDebug = true;
    m_fKeyFrame = 0.f;

    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
    m_fRange = 1.f;
    m_vPosition = _float3(0.f, 0.f, 0.f);

#endif // _DEBUG


    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

void CBody_Player::Update(_float fTimeDelta)
{
    if(m_isDebug)
        m_isAnimationFinish = m_pModelCom->Play_Animation(fTimeDelta * (*m_pAttackSpeed));
    else
        m_isAnimationFinish = m_pModelCom->Play_Debug_Animation(m_fKeyFrame);
   
    //m_isAnimationFinish = m_pModelCom->Play_Animation(fTimeDelta * (*m_pAttackSpeed));

    /* 부모 행렬 적용 */
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));

    /* 카메라 타겟 설정 */
    XMStoreFloat4x4(&m_CameraTargetBoneWorldMatrix,
        XMLoadFloat4x4(m_pCameraTargetBoneMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));

    if (m_bApplyRimLight || m_fRimStrength > 0.f)
    {
        m_fRimStrength -= fTimeDelta * 0.5f;
        if (m_fRimStrength < 0.f)
        {
            m_fRimStrength = 0.f;
            m_bApplyRimLight = false;
        }
    }

#ifdef _DEBUG

#endif // _DEBUG

}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(RENDER::SHADOW, this);
}

HRESULT CBody_Player::Render()
{
#ifdef _DEBUG
  /*  ImGui::InputFloat3("D", (_float*)(&LightDesc.vDiffuse));
    ImGui::InputFloat3("A", (_float*)(&LightDesc.vAmbient));
    ImGui::InputFloat3("S", (_float*)(&LightDesc.vSpecular));
    ImGui::InputFloat3("LightPos", (_float*)(&LightDesc.vDirection));

    m_pGameInstance->Update_Light_Color(L"Dircection", 0, &LightDesc.vDiffuse);
    m_pGameInstance->Update_Light_Color(L"Dircection", 1, &LightDesc.vAmbient);
    m_pGameInstance->Update_Light_Color(L"Dircection", 2, &LightDesc.vSpecular);
    m_pGameInstance->Update_Light_Position(L"Dircection", reinterpret_cast<_float3*>(&LightDesc.vDirection));
*/

   // ImGui::InputFloat("PointRange", &m_fRange);

    //m_pGameInstance->Update_Light_Range(L"Point1", m_fRange);


    //ImGui::InputFloat3("D", (_float*)(&LightDesc.vDiffuse));
    //ImGui::InputFloat3("A", (_float*)(&LightDesc.vAmbient));
    //ImGui::InputFloat3("S", (_float*)(&LightDesc.vSpecular));
    //ImGui::InputFloat3("LightPos", (_float*)(&m_vPosition));
#endif // _DEBUG


#pragma region ANIMATION_TEST
    ImGui::Checkbox("Play", &m_isDebug);
    ImGui::DragFloat("KeyFrame", &m_fKeyFrame, 0.1f, 0.f, 300.f, "%.3f");
    //ImGui::DragFloat("SoundKey", &m_fSoundKey);
    //ImGui::DragFloat("m_fVolume", &m_fVolume);
    //ImGui::DragFloat("m_fTestSoundKey", &m_fTestSoundKey);

    //if(ImGui::BeginCombo("SoundName", ""))
    //{

    //    for (int i = 0; i < m_SoundNames.size(); ++i)
    //    {
    //        bool isSelected = (m_currentIndex == i);
    //        if (ImGui::Selectable(m_SoundNames[i].c_str(), isSelected))
    //        {
    //            m_currentIndex = i;
    //            ImGui::SetClipboardText(m_SoundNames[i].c_str());
    //        }

    //        if (isSelected)
    //            ImGui::SetItemDefaultFocus();
    //    }

    //    ImGui::EndCombo();
    //}
    //_float fFrame = m_pModelCom->Get_TrackPosition();
    //if ((m_isDebug && m_fTestSoundKey <= fFrame) && (false == m_isTrigge))
    //{
    //    m_isTrigge = true;
    //    m_pGameInstance->Play_Sound(m_pGameInstance->Utf8ToWstring(m_SoundNames[m_currentIndex].c_str()).c_str(), CHANNELID::SKILL_PLAYER, m_fVolume);
    //}

  
    /*ImGui::InputInt("Animation", &m_iAnimIndex);
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
    }*/
#pragma endregion
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pParentTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_NormalTexture", TEXTURE::NORMAL, 0)))
        {
            if (FAILED(m_pShaderCom->Begin(0)))
                return E_FAIL;
        }
        else
        {
            if(false == m_bApplyRimLight && m_fRimStrength <= 0.f)
            {
                if (FAILED(m_pShaderCom->Begin(2)))
                    return E_FAIL;
            }
            else
            {
                if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
                    return E_FAIL;

                if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &m_vRimColor[ENUM_TO_INT(*m_pStance)], sizeof(_float4))))
                    return E_FAIL;

                if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimStrength", &m_fRimStrength, sizeof(_float))))
                    return E_FAIL;

                if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimPower", &m_fRimPower, sizeof(_float))))
                    return E_FAIL;

                if (FAILED(m_pShaderCom->Begin(3)))
                    return E_FAIL;
            }
        }
            
        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CBody_Player::Render_Shadow()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pParentTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_Shadow_Resource(m_pShaderCom, "g_ViewMatrix", D3DTS::VIEW)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_Shadow_Resource(m_pShaderCom, "g_ProjMatrix", D3DTS::PROJ)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(4)))
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

HRESULT CBody_Player::Bind_CameraBoneMatrix()
{
    /* Fix */
    CCamera* pCamera = m_pGameInstance->Find_Camera(TEXT("Camera_Fix"));

    if (nullptr == pCamera)
        return E_FAIL;

    pCamera->Set_CameraTargetBone(&m_CameraTargetBoneWorldMatrix);

    /* ChargeSkill */
   pCamera = m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"));

    if (nullptr == pCamera)
        return E_FAIL;

    pCamera->Set_CameraTargetBone(&m_CameraTargetBoneWorldMatrix);

    /* Kamen */
   pCamera = m_pGameInstance->Find_Camera(TEXT("Camera_Kamen_Intro"));

    if (nullptr == pCamera)
        return E_FAIL;

    pCamera->Set_CameraTargetBone(&m_CameraTargetBoneWorldMatrix);

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
