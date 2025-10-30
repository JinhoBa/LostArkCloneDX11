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

    m_fLerp = 0.f;
    m_fFovy = XMConvertToRadians(60.f);
    m_fClashFovy = XMConvertToRadians(46.f);

    return S_OK;
}

void CCamera_Clash::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position(fTimeDelta);

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

    return S_OK;
}

void CCamera_Clash::Reset()
{
    m_fLerp = 0.f;
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));
}

void CCamera_Clash::Set_Position(_fvector vPosistion)
{
    _float3 vPlayerPosition;
    XMStoreFloat3(&vPlayerPosition, vPosistion);

    m_vPlayerPos = vPlayerPosition;

    XMStoreFloat3(&m_vStartPosition, XMVectorSet(
        vPlayerPosition.x - 1.7f, vPlayerPosition.y + 5.f, vPlayerPosition.z - 1.0f, 1.f));

    XMStoreFloat3(&m_vEndPosition, XMVectorSet(
        vPlayerPosition.x + 1.8f, vPlayerPosition.y + 1.f, vPlayerPosition.z - 1.5f, 1.f));

    XMStoreFloat3(&m_vClashingPosition, XMVectorSet(
        m_vEndPosition.x -0.2f, m_vEndPosition.y - 0.6f, m_vEndPosition.z - 0.5f, 1.f));

}

void CCamera_Clash::Update_Camera_Position(_float fTimeDelta)
{
    m_fLerp += fTimeDelta;

    switch (m_eState)
    {
    case Client::CCamera_Clash::CLASH_CAMERA::START:
        if (4.f <= m_fLerp)
            m_fLerp = 4.f;


        m_pTransformCom->Set_State(STATE::POSITION,
            XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_vStartPosition), XMLoadFloat3(&m_vEndPosition), m_fLerp / 4.f), 1.f));

        memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

        m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));

        if(4.f == m_fLerp)
        {
            m_eState = CLASH_CAMERA::LOOP;
            m_fLerp = 0.f;
        }

        break;

    case Client::CCamera_Clash::CLASH_CAMERA::LOOP:
        if (2.f <= m_fLerp)
            m_fLerp = 2.f;

        m_fFovy = XMConvertToRadians(60.f) * (1.f - m_fLerp / 1.f) + XMConvertToRadians(46.f) * (m_fLerp / 1.f);

        m_pTransformCom->Set_State(STATE::POSITION,
            XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_vEndPosition), XMLoadFloat3(&m_vClashingPosition), m_fLerp / 2.f), 1.f));

        memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

        m_pTransformCom->LookAt(XMVectorSet(
            m_vTargetPosition.x,
            m_vTargetPosition.y + 1.5f,
            m_vTargetPosition.z, 1.f));

        break;
    }


   
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
