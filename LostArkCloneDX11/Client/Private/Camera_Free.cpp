#include "pch.h"
#include "Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{  

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
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



    __super::Bind_Transform();
}

void CCamera_Free::Update(_float fTimeDelta)
{
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_Free::Render()
{


    return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_Free");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Free(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_Free");
        return nullptr;
    }

    return pInstance;
}

void CCamera_Free::Free()
{
    __super::Free();
}
