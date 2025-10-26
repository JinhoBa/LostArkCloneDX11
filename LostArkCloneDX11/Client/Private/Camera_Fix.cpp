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

void CCamera_Fix::Start_Shake(_float fTime)
{
    m_isShake = true;
    m_fDuration = 0.2f;
    m_fChangeDuration = 0.2f;
    m_fTimeAcc = 0.f;
}

HRESULT CCamera_Fix::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Fix::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    __super::Bind_Transform();

    m_vTargetPosition = _float3(0.f, 0.f, 0.f);

    m_isShake = false;

    return S_OK;
}

void CCamera_Fix::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position(fTimeDelta);

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
    return S_OK;
}

void CCamera_Fix::Reset()
{
    if (nullptr == m_pCameraTargetBoneMatrix)
        return;

    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));
}

void CCamera_Fix::Update_Camera_Position(_float fTimeDelta)
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION, 
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));

    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));

    Shake(fTimeDelta);
}

void CCamera_Fix::Change_State()
{

}

void CCamera_Fix::Shake(_float fTimeDelta)
{
    if (!m_isShake)
        return;

    if(0.f == m_fTimeAcc)
    {
        _vector vRight = m_pTransformCom->Get_State(STATE::RIGHT) * m_pGameInstance->Random(-1.f, 1.f);
        _vector vUp = m_pTransformCom->Get_State(STATE::UP) * m_pGameInstance->Random(-1.f, 1.f);
  
        XMStoreFloat4(&m_vPositionOffset, XMVector3Normalize(vRight + vUp));
    }

    m_fTimeAcc += fTimeDelta;
    m_fChangeDuration += fTimeDelta;

    _vector vDir = XMVectorLerp(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMLoadFloat4(&m_vPositionOffset), sin(m_fTimeAcc * 8.f * XM_PI));

    m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_Position() + vDir * 0.05f);

    if (m_fTimeAcc >= m_fDuration)
        m_isShake = false;
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
