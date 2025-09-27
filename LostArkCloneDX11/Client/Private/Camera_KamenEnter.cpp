#include "pch.h"
#include "Camera_Kamen_Intro.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Camera_KamenEnter.h"

CCamera_KamenEnter::CCamera_KamenEnter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_KamenEnter::CCamera_KamenEnter(const CCamera_KamenEnter& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_KamenEnter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_KamenEnter::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_fFovy = XMConvertToRadians(60.f);

    const CAMERA_ANIMATION_DESC* pDesc = CGameManager::GetInstance()->Get_CameraAnimation(0);

    memcpy(&m_anim[0], pDesc, sizeof(CAMERA_ANIMATION_DESC));

    pDesc = CGameManager::GetInstance()->Get_CameraAnimation(1);

    memcpy(&m_anim[1], pDesc, sizeof(CAMERA_ANIMATION_DESC));

    pDesc = CGameManager::GetInstance()->Get_CameraAnimation(2);

    memcpy(&m_anim[2], pDesc, sizeof(CAMERA_ANIMATION_DESC));

    return S_OK;
}

void CCamera_KamenEnter::Priority_Update(_float fTimeDelta)
{
    //Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_KamenEnter::Update(_float fTimeDelta)
{
    /* ½ºÅµ */
    if (m_pGameInstance->Get_KeyDown(DIK_ESCAPE))
    {
        End_Scene();
        return;
    }

    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc * m_anim[m_iAnimationIndex].fSpeed < m_anim[m_iAnimationIndex].fDuration)
    {
        m_pTransformCom->Set_State(STATE::POSITION,
            XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_anim[m_iAnimationIndex].vStartPositon),
                XMLoadFloat3(&m_anim[m_iAnimationIndex].vEndPosition), m_fTimeAcc * m_anim[m_iAnimationIndex].fSpeed / m_anim[m_iAnimationIndex].fDuration), 1.f));

        if (0.f != m_anim[m_iAnimationIndex].fRotationSpeed)
            m_pTransformCom->Turn(XMLoadFloat3(&m_anim[m_iAnimationIndex].vRotationAxis), m_anim[m_iAnimationIndex].fRotationSpeed * fTimeDelta);
    }
    /* else if (m_anim[m_iAnimationIndex].isLoop)
         m_fTimeAcc = 0.f;*/
    else if (m_iAnimationIndex < 2)
    {
        m_fTimeAcc = 0.f;
        m_iAnimationIndex++;
    }

   if (2 == m_iAnimationIndex && m_fTimeAcc * m_anim[m_iAnimationIndex].fSpeed >= m_anim[m_iAnimationIndex].fDuration)
   {
       End_Scene();
   }

   if(m_iAnimationIndex < 3)
        m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&(m_anim[m_iAnimationIndex].vTargetPosition)), 1.f));
}

void CCamera_KamenEnter::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CCamera_KamenEnter::Render()
{
    ImGui::Checkbox("isLoop", &m_anim[m_iAnimationIndex].isLoop);
    //
    //ImGui::InputFloat("fDuration", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].fDuration));
    //ImGui::InputFloat("fSpeed", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].fSpeed));
    //ImGui::InputFloat("fStartFov", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].fFov));
    //ImGui::InputFloat("fRotationSpeed", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].fRotationSpeed));

    //ImGui::InputFloat3("vStartPositon", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].vStartPositon));
    //ImGui::InputFloat3("vEndPosition", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].vEndPosition));
    //ImGui::InputFloat3("vRotationAxis", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].vRotationAxis));
    //ImGui::InputFloat3("vTargetPosition", reinterpret_cast<_float*>(&m_anim[m_iAnimationIndex].vTargetPosition));



    return S_OK;
}

void CCamera_KamenEnter::Reset()
{
    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_anim[m_iAnimationIndex].vStartPositon), 1.f));

    m_vTargetPosition = m_anim[m_iAnimationIndex].vTargetPosition;

    m_fTimeAcc = 0.f;
}


void CCamera_KamenEnter::End_Scene()
{
    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_anim[2].vEndPosition), 1.f));
    m_vTargetPosition = m_anim[m_iAnimationIndex].vTargetPosition;
    m_pGameInstance->Bind_Camera(TEXT("Camera_Kamen_Intro"), false, 2.f);
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Kamen_Sword"),
        ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Monster"))))
        return;
}

CCamera_KamenEnter* CCamera_KamenEnter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_KamenEnter* pInstance = new CCamera_KamenEnter(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_KamenEnter");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_KamenEnter::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_KamenEnter(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_KamenEnter");
        return nullptr;
    }

    return pInstance;
}

void CCamera_KamenEnter::Free()
{
    __super::Free();
}
