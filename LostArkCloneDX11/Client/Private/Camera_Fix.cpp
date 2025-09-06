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

HRESULT CCamera_Fix::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Fix::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_vDistance = _float3(0.f, 5.f, -5.f);
    m_eCurState = m_ePreState = CAMERA_ANIM::IDLE;
    m_fTimeAcc = 0.f;
    CGameManager::GetInstance()->Set_Camera(this);

    return S_OK;
}

void CCamera_Fix::Priority_Update(_float fTimeDelta)
{
#pragma region TESTCODE
    /*Move*/
    if (m_pGameInstance->Get_KeyPressing(DIK_D))
        m_pTransformCom->Go_Right(fTimeDelta);
    if (m_pGameInstance->Get_KeyPressing(DIK_A))
        m_pTransformCom->Go_Left(fTimeDelta);
    if (m_pGameInstance->Get_KeyPressing(DIK_W))
        m_pTransformCom->Go_Straight_World(fTimeDelta);
    if (m_pGameInstance->Get_KeyPressing(DIK_S))
        m_pTransformCom->Go_Backward_World(fTimeDelta);

    /*Zoom In/Out */
    if (m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::W) > 0)
        m_pTransformCom->Go_Straight(fTimeDelta * 10.f);
    if (m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::W) < 0)
        m_pTransformCom->Go_Backward(fTimeDelta * 10.f);

    /* Rotation */
    if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::WBUTTON))
    {
        long lValue = {};
        if (lValue = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
        {
            m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * lValue * 0.1f);
        }
        if (lValue = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
        {
            m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fTimeDelta * lValue * 0.1f);
        }
    }

    Update_Camera_Position();

    __super::Bind_Transform();
#pragma endregion

}

void CCamera_Fix::Update(_float fTimeDelta)
{
    Change_State();

    switch (m_eCurState)
    {   
    case Client::CAMERA_ANIM::IDLE:

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
             m_vDistance.z -= fTimeDelta* 0.5f;
             m_vDistance.y += fTimeDelta * 0.5f;
         }
        else
            m_eCurState = CAMERA_ANIM::IDLE;
        break;

    default:
        break;
    }

}

void CCamera_Fix::Late_Update(_float fTimeDelta)
{
   
}

HRESULT CCamera_Fix::Render()
{

    return S_OK;
}

void CCamera_Fix::Update_Camera_Position()
{
    if (CAMERA_ANIM::SHAKE == m_eCurState)
    {
        _float4 fRandomPosition = _float4(
            m_pTargetPosition.x + m_pGameInstance->Random(-0.3f, 0.3f),
            m_pTargetPosition.y + m_pGameInstance->Random(-0.3f, 0.3f),
            m_pTargetPosition.z + m_pGameInstance->Random(-0.3f, 0.3f), 1.f
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
            m_vDistance = _float3(0.f, 5.f, -5.f);
            break;

        case CAMERA_ANIM::SHAKE:
            m_fTimeAcc = 0.f;
            m_fDuration = 1.f;

            break;
        case CAMERA_ANIM::ZOOMOUT:
            m_fTimeAcc = 0.f;
            m_fDuration = 1.f;
 
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
