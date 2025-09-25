#include "pch.h"
#include "Camera_Kamen_Intro.h"

#include "GameInstance.h"
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



    return S_OK;
}

void CCamera_KamenEnter::Priority_Update(_float fTimeDelta)
{
    //Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_KamenEnter::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fTimeAcc * m_anim.fSpeed < m_anim.fDuration)
    {
        m_pTransformCom->Set_State(STATE::POSITION,
            XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_anim.vStartPositon),
                XMLoadFloat3(&m_anim.vEndPosition), m_fTimeAcc * m_anim.fSpeed / m_anim.fDuration),1.f));

        if (0.f != m_anim.fRotationSpeed)
            m_pTransformCom->Turn(XMLoadFloat3(&m_anim.vRotationAxis), m_anim.fRotationSpeed * fTimeDelta);
    }
    else if (m_anim.isLoop)
        m_fTimeAcc = 0.f;
}

void CCamera_KamenEnter::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CCamera_KamenEnter::Render()
{
    ImGui::Checkbox("isLoop", &m_anim.isLoop);

    ImGui::InputFloat("fDuration", reinterpret_cast<_float*>(&m_anim.fDuration));
    ImGui::InputFloat("fSpeed", reinterpret_cast<_float*>(&m_anim.fSpeed));
    ImGui::InputFloat("fStartFov", reinterpret_cast<_float*>(&m_anim.fStartFov));
    ImGui::InputFloat("fEndFov", reinterpret_cast<_float*>(&m_anim.fEndFov));
    ImGui::InputFloat("fRotationSpeed", reinterpret_cast<_float*>(&m_anim.fRotationSpeed));

    ImGui::InputFloat3("vStartPositon", reinterpret_cast<_float*>(&m_anim.vStartPositon));
    ImGui::InputFloat3("vEndPosition", reinterpret_cast<_float*>(&m_anim.vEndPosition));
    ImGui::InputFloat3("vRotationAxis", reinterpret_cast<_float*>(&m_anim.vRotationAxis));
    ImGui::InputFloat3("vTargetPosition", reinterpret_cast<_float*>(&m_anim.vTargetPosition));



    return S_OK;
}

void CCamera_KamenEnter::Reset()
{
    m_fTimeAcc = 0.f;

}

void CCamera_KamenEnter::Update_Camera_Position()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float4));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMLoadFloat4(&m_vTargetPosition)
        + XMLoadFloat3(&m_vDirection));

    m_pTransformCom->LookAt(XMLoadFloat4(&m_vTargetPosition));
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
