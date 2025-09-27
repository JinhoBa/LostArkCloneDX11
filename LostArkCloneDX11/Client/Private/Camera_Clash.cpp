#include "pch.h"
#include "Camera_Clash.h"

#include "GameInstance.h"
#include "GameManager.h"


CCamera_Clash::CCamera_Clash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Clash::CCamera_Clash(const CCamera_Clash& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_Clash::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Clash::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    __super::Bind_Transform();

    m_vTargetPosition = _float3(0.f, 0.f, 0.f);

    return S_OK;
}

void CCamera_Clash::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_Clash::Update(_float fTimeDelta)
{
}

void CCamera_Clash::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Clash::Render()
{
    /*
    _float fFovy = XMConvertToDegrees(m_fFovy);

    ImGui::InputFloat("Fovy", &fFovy, 0.1f, 1.f);
    ImGui::InputFloat("X", &m_vDistance.x, 0.1f, 1.f);
    ImGui::InputFloat("Y", &m_vDistance.y, 0.1f, 1.f);
    ImGui::InputFloat("Z", &m_vDistance.z, 0.1f, 1.f);

    m_fFovy = XMConvertToRadians(fFovy);*/

    return S_OK;
}

void CCamera_Clash::Reset()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));
}

void CCamera_Clash::Update_Camera_Position()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));

    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));
}

void CCamera_Clash::Change_State()
{
    /*   if (m_ePreState != m_eCurState)
       {
           switch (m_eCurState)
           {
           case CAMERA_ANIM::IDLE:
               m_fTimeAcc = 0.f;
               m_fFovy = XMConvertToRadians(60.f);
               m_vDistance = m_Default_Direction;
               break;

           case CAMERA_ANIM::INTOR_BOSS:
               m_fTimeAcc = 0.f;
               m_fFovy = XMConvertToRadians(40.f);
               m_vDistance = m_Default_Direction = _float3(0.f, 3.2f, -13.3f);
               break;

           case CAMERA_ANIM::SHAKE:
               m_fTimeAcc = 0.f;
               m_fDuration = 1.f;

               break;
           case CAMERA_ANIM::ZOOMOUT:
               m_fTimeAcc = 0.f;
               m_fDuration = 1.f;
               m_eLevelState = m_ePreState;
               break;


           default:
               break;
           }
           m_ePreState = m_eCurState;
       }*/

}

CCamera_Clash* CCamera_Clash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Clash* pInstance = new CCamera_Clash(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_Clash");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Clash::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Clash(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_Clash");
        return nullptr;
    }

    return pInstance;
}

void CCamera_Clash::Free()
{
    __super::Free();
}
