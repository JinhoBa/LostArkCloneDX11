#include "pch.h"
#include "Camera_Kamen_Intro.h"

#include "GameInstance.h"

CCamera_Kamen_Intro::CCamera_Kamen_Intro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Kamen_Intro::CCamera_Kamen_Intro(const CCamera_Kamen_Intro& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_Kamen_Intro::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Kamen_Intro::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void CCamera_Kamen_Intro::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_Kamen_Intro::Update(_float fTimeDelta)
{
}

void CCamera_Kamen_Intro::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_Kamen_Intro::Render()
{

    return S_OK;
}

void CCamera_Kamen_Intro::Reset()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));
    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));
}

void CCamera_Kamen_Intro::Update_Camera_Position()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION, 
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));

    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));
}


CCamera_Kamen_Intro* CCamera_Kamen_Intro::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Kamen_Intro* pInstance = new CCamera_Kamen_Intro(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_Kamen_Intro");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Kamen_Intro::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Kamen_Intro(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_Kamen_Intro");
        return nullptr;
    }

    return pInstance;
}

void CCamera_Kamen_Intro::Free()
{
    __super::Free();
}
