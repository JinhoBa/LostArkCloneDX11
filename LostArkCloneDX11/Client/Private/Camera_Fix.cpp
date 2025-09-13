#include "pch.h"
#include "Camera_Fix.h"

#include "GameInstance.h"
#include "GameManager.h"


CCamera_Fix::CCamera_Fix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Fix::CCamera_Fix(const CCamera_Fix& Prototype)
    :CCamera{ Prototype }
{
}

void CCamera_Fix::Set_CameraTargetPosition(_vector TargetPosition)
{
    XMStoreFloat4(&m_pTargetPosition, TargetPosition);
}

void CCamera_Fix::Set_LookDircetion(_fvector vDirection)
{
    XMStoreFloat3(&m_Default_Direction, vDirection);
    XMStoreFloat3(&m_vDistance, vDirection);
}

HRESULT CCamera_Fix::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Fix::Initialize(void* pArg)
{
    m_pTargetPosition = _float4(0.f, 0.f, 0.f, 1.f);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_Default_Direction = m_vDistance = _float3(0.f, 5.f, 5.f);

    m_eCurState = m_ePreState = CAMERA_ANIM::IDLE;
    m_fTimeAcc = 0.f;
    CGameManager::GetInstance()->Set_Camera(this);

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    __super::Bind_Transform();



    return S_OK;
}

void CCamera_Fix::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_Fix::Update(_float fTimeDelta)
{
    Change_State();

    switch (m_eCurState)
    {   
    case Client::CAMERA_ANIM::IDLE:

        break;

    case Client::CAMERA_ANIM::INTOR_BOSS:

        break;

    case Client::CAMERA_ANIM::SHAKE:
        m_fTimeAcc += fTimeDelta;

        if (m_fTimeAcc >= m_fDuration)
            m_eCurState = CAMERA_ANIM::IDLE;

        break;

    case Client::CAMERA_ANIM::ZOOMOUT:
         m_fTimeAcc += fTimeDelta;
         if (m_fTimeAcc < m_fDuration)
         {
             if(0.f < m_Default_Direction.z)
                m_vDistance.z += fTimeDelta* 0.5f;
             else
                 m_vDistance.z -= fTimeDelta * 0.5f;

             m_vDistance.y += fTimeDelta * 0.5f;
         }
        else
            m_eCurState = m_eLevelState;
        break;

    default:
        break;
    }

}

void CCamera_Fix::Late_Update(_float fTimeDelta)
{
 /*   m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);*/
}

HRESULT CCamera_Fix::Render()
{/*
    _float fFovy = XMConvertToDegrees(m_fFovy);
   
    ImGui::InputFloat("Fovy", &fFovy, 0.1f, 1.f);
    ImGui::InputFloat("X", &m_vDistance.x, 0.1f, 1.f);
    ImGui::InputFloat("Y", &m_vDistance.y, 0.1f, 1.f);
    ImGui::InputFloat("Z", &m_vDistance.z, 0.1f, 1.f);

    m_fFovy = XMConvertToRadians(fFovy);*/

    return S_OK;
}

void CCamera_Fix::Update_Camera_Position()
{
    if (CAMERA_ANIM::SHAKE == m_eCurState)
    {
        _float4 fRandomPosition = _float4(
            m_pTargetPosition.x + m_pGameInstance->Random(-0.1f, 0.1f),
            m_pTargetPosition.y,
            m_pTargetPosition.z + m_pGameInstance->Random(-0.1f, 0.1f), 1.f
        );

        m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&fRandomPosition) + XMLoadFloat3(&m_vDistance));
    }
    else
        m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&m_pTargetPosition) + XMLoadFloat3(&m_vDistance));

    m_pTransformCom->LookAt(XMLoadFloat4(&m_pTargetPosition));
}

void CCamera_Fix::Change_State()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case CAMERA_ANIM::IDLE:
            m_fTimeAcc = 0.f;
            m_fFovy = XMConvertToRadians(60.f);
            m_vDistance = m_Default_Direction = _float3(0.f, 5.f, 5.f);;
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
    }

}

CCamera_Fix* CCamera_Fix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Fix* pInstance = new CCamera_Fix(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_Fix");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Fix::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Fix(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_Fix");
        return nullptr;
    }

    return pInstance;
}

void CCamera_Fix::Free()
{
    __super::Free();
}
