#include "pch.h"
#include "Camera_ChargeSkill.h"

#include "GameInstance.h"

CCamera_ChargeSkill::CCamera_ChargeSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_ChargeSkill::CCamera_ChargeSkill(const CCamera_ChargeSkill& Prototype)
    :CCamera{ Prototype }
{
}


HRESULT CCamera_ChargeSkill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_ChargeSkill::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    __super::Bind_Transform();

    return S_OK;
}

void CCamera_ChargeSkill::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_KeyUp(DIK_D))
    {
        m_isEnd = true;
    }

    if(false == m_isEnd)
    {
        m_fScala += fTimeDelta * 0.3f;

        if (1.3f < m_fScala)
            m_fScala = 1.3f;
    }
    else
    {
        m_fScala -= fTimeDelta *0.5f;
        if (1.f > m_fScala)
        {
            m_fScala = 1.f;
        }
    }

    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_ChargeSkill::Update(_float fTimeDelta)
{
 
}

void CCamera_ChargeSkill::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_ChargeSkill::Render()
{
    return S_OK;
}

void CCamera_ChargeSkill::Reset()
{
    m_isEnd = false;
    m_fTimeAcc = 0.f;
    m_fScala = 1.f;
    m_eState = CAMERASTATE::ZOOMOUT;

    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));
}

void CCamera_ChargeSkill::Update_Camera_Position()
{
    memcpy(&m_vTargetPosition, m_pCameraTargetBoneMatrix->m[3], sizeof(_float3));

    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection) * m_fScala);

    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));
}


CCamera_ChargeSkill* CCamera_ChargeSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_ChargeSkill* pInstance = new CCamera_ChargeSkill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_ChargeSkill");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_ChargeSkill::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_ChargeSkill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_ChargeSkill");
        return nullptr;
    }

    return pInstance;
}

void CCamera_ChargeSkill::Free()
{
    __super::Free();
}
