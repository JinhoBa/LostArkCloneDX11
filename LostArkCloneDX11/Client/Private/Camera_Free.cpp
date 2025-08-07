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
    if (m_pGameInstance->KeyPressing(VK_RIGHT))
        m_pTransformCom->Go_Right(fTimeDelta);
    if (m_pGameInstance->KeyPressing(VK_LEFT))
        m_pTransformCom->Go_Left(fTimeDelta);
    if (m_pGameInstance->KeyPressing(VK_UP))
        m_pTransformCom->Go_Straight(fTimeDelta);
    if (m_pGameInstance->KeyPressing(VK_DOWN))
        m_pTransformCom->Go_Backward(fTimeDelta);
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
