#include "pch.h"
#include "Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "PartObject.h"

CKamen::CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCharacter{ pDevice, pContext }
{
}

CKamen::CKamen(const CKamen& Prototype)
    :CCharacter{ Prototype }
{
}

HRESULT CKamen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CKamen::Initialize(void* pArg)
{
    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 5.f;
    Desc.fSpeedPersec = 5.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Reay_Component()))
        return E_FAIL;

    if (FAILED(Reay_States()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(40.f, 0.f, 40.f, 1.f));

    return S_OK;
}

void CKamen::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CKamen::Update(_float fTimeDelta)
{

    __super::Update(fTimeDelta);

}

void CKamen::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CKamen::Render()
{

    return S_OK;
}

HRESULT CKamen::Reay_Component()
{
    /* StateMachine */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
        TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CKamen::Reay_States()
{

  /*  m_States[IDLE] = CPlayer_Idle::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[MOVE] = CPlayer_Move::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[NORMAL_SKILL] = CPlayer_NormalSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CHARGE_SKILL] = CPlayer_ChargeSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[COMBO_SKILL] = CPlayer_ComboSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[DASH] = CPlayer_Dash::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CHANGE_STANCE] = CPlayer_ChangeStance::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[HIT] = CPlayer_Hit::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);*/

    return S_OK;
}

HRESULT CKamen::Ready_PartObjects()
{
    CPartObject::PARTOBJECT_DESC Body_Desc = {};
    Body_Desc.pParentTransform = m_pTransformCom;
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Kamen"), TEXT("Body_Kamen"), &Body_Desc)))
        return E_FAIL;


    return S_OK;
}

CKamen* CKamen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CKamen* pInstance = new CKamen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CKamen");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CKamen::Clone(void* pArg)
{
    CGameObject* pInstance = new CKamen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CKamen");
        return nullptr;
    }

    return pInstance;
}

void CKamen::Free()
{
    __super::Free();

}
