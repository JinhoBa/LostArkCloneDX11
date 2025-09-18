#include "pch.h"
#include "Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Idle_Kamen.h"
#include "Intro_Kamen.h"
#include "Attack_Normal_Kamen.h"
#include "Attack_Combo_Kamen.h"
#include "Attack_Charge_Kamen.h"

#include "Body_Kamen.h"
#include "Weapon_Kamen.h"

CKamen::CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEnemy{ pDevice, pContext }
{
}

CKamen::CKamen(const CKamen& Prototype)
    :CEnemy{ Prototype }
{
}

_bool CKamen::isAnimationFinish()
{
    return static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->isAnimationFinish();
}

void CKamen::Set_Animation(_uint iIndex, _bool bLoop, _float fLerpTime)
{
    static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Set_Animation(iIndex, bLoop, fLerpTime);
}

void CKamen::Change_Phase(PHASE ePhase)
{
    m_ePhase = ePhase;

    CAttack_Kamen::ATTACK_KAMEN_DESC Desc = {};

    switch (ePhase)
    {
    case Client::PHASE::PHASE1:

        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 60.f, 1.f));
        m_pStateMachineCom->Change_State(Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
        break;

    case Client::PHASE::PHASE2:
        break;
    case Client::PHASE::PHASE3:
        break;
    case Client::PHASE::END:
        break;
    default:
        break;
    }

}

void CKamen::Chase(_float fTimeDelta)
{
    _vector vToTarget = (m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position());

    m_pTransformCom->Chase(fTimeDelta, XMVector3Normalize(vToTarget), m_pTransformCom->Get_Position() + vToTarget, 3.f, m_pNavigationCom);
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

    m_ePhase = PHASE::INTRO;

    m_EnemyInfo.fAttack = 600.f;
    m_EnemyInfo.fAttackRange = 3.f;
    m_EnemyInfo.fDetectDistance = 5.f;
    m_EnemyInfo.fHp = m_EnemyInfo.fMaxHp = 500000.f;

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 2.9f, 71.f, 1.f));
    m_pTransformCom->Rotation(0.f, XMConvertToRadians(180.f), 0.f);
    m_pStateMachineCom->Start_State(m_States[ENUM_TO_INT(KAMENSTATE::INTRO)]);

    return S_OK;
}

void CKamen::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CKamen::Update(_float fTimeDelta)
{
    m_pStateMachineCom->Upadte(fTimeDelta);

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

void CKamen::OnHit(const ATTACK_DESC& Attack_Desc)
{
    m_EnemyInfo.fHp -= Attack_Desc.fDamage;

  
}

HRESULT CKamen::Reay_Component()
{
    /* StateMachine */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
        TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
        return E_FAIL;

    CNavigation::NAVIGATION_DESC Navi_Desc = {};

    Navi_Desc.iCurrentIndex = 0;

    /* Navigation */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Kamen"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Navi_Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CKamen::Reay_States()
{
    CState_Kamen::STATE_KAMEN_DESC Desc = {};

    Desc.pKamen = this;
    Desc.pPhase = &m_ePhase;
    Desc.pStateMachine = m_pStateMachineCom;

    m_States[ENUM_TO_INT(KAMENSTATE::INTRO)] = CIntro_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::IDLE)] = CIdle_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::ATTACK_NORMAL)] = CAttack_Normal_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::ATTACK_COMBO)] = CAttack_Combo_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::ATTACK_CHARGE)] = CAttack_Charge_Kamen::Create(&Desc);


    return S_OK;
}

HRESULT CKamen::Ready_PartObjects()
{
    CPartObject::PARTOBJECT_DESC Body_Desc = {};
    Body_Desc.pParentTransform = m_pTransformCom;
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), 
        TEXT("Prototype_GameObject_Body_Kamen"), TEXT("Body_Kamen"), &Body_Desc)))
        return E_FAIL;

    CWeapon_Kamen::WEAPON_KAMEN_DESC Weapon_Desc = {};
    Weapon_Desc.pParentTransform = m_pTransformCom;
    Weapon_Desc.pSocketMatrix = dynamic_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_wpn_02");

    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), 
        TEXT("Prototype_GameObject_Weapon_Kamen"), TEXT("Weapon_Kamen"), &Weapon_Desc)))
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
