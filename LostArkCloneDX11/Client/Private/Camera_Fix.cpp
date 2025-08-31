#include "pch.h"
#include "Camera_Fix.h"

#include "GameInstance.h"

CCamera_Fix::CCamera_Fix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Fix::CCamera_Fix(const CCamera_Fix& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_Fix::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Fix::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (nullptr == m_pPlayerTransformCom)
        return E_FAIL;

    m_vDistance = _float3(0.f, 5.f, -5.f);



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
#pragma endregion

    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_Fix::Update(_float fTimeDelta)
{
 
}

void CCamera_Fix::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_Fix::Render()
{
    ImGui::InputFloat3("",reinterpret_cast<_float*>(&m_vDistance));

    return S_OK;
}

void CCamera_Fix::Update_Camera_Position()
{
    _vector vPlayerPosition = m_pPlayerTransformCom->Get_Position();

    m_pTransformCom->Set_State(STATE::POSITION, vPlayerPosition + XMLoadFloat3(&m_vDistance));

    m_pTransformCom->LookAt(vPlayerPosition);
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
