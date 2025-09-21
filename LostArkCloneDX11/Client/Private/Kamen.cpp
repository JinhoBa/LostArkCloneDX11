#include "pch.h"
#include "Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Idle_Kamen.h"
#include "Intro_Kamen.h"
#include "Attack_Normal_Kamen.h"
#include "Attack_Combo_Kamen.h"
#include "Attack_Charge_Kamen.h"
#include "Attack_Sword_Kamen.h"
#include "Attack_Spin_Kamen.h"

#include "Body_Kamen.h"
#include "Weapon_Kamen.h"
#include "Player.h"

CKamen::CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEnemy{ pDevice, pContext }
{
}

CKamen::CKamen(const CKamen& Prototype)
    :CEnemy{ Prototype }
{
}

_float CKamen::Get_TrackPositon()
{
    return static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_TrackPoisiton();
}

_bool CKamen::isAnimationFinish()
{
    return static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->isAnimationFinish();
}

void CKamen::Set_Animation(_uint iIndex, _bool bLoop, _float fLerpTime)
{
    static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Set_Animation(iIndex, bLoop, fLerpTime);
}

void	CKamen::Set_HitBox(_float3& vCenter, _float3& vExtends)
{
    m_pHitBoxCom->Set_ColliderDesc(vCenter, vExtends);
}

void CKamen::Change_Phase(PHASE ePhase)
{
    m_ePhase = ePhase;

    CAttack_Kamen::ATTACK_KAMEN_DESC Desc = {};

    switch (ePhase)
    {
    case Client::PHASE::PHASE1:

        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 50.f, 1.f));
        m_pStateMachineCom->Change_State(Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
        static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Set_Animation(59, false, 0.2f);
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
    m_EnemyInfo.fHp = m_EnemyInfo.fMaxHp = 500000000.f;

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    m_pStateMachineCom->Start_State(m_States[ENUM_TO_INT(KAMENSTATE::INTRO)]);
     m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 2.9f, 71.f, 1.f));
    m_pTransformCom->Rotation(0.f, XMConvertToRadians(180.f), 0.f);
   

    return S_OK;
}

void CKamen::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    isCollUpdate = false;

    m_pGameInstance->Add_Collider(TEXT("Monster"), m_pColliderCom);
}

void CKamen::Update(_float fTimeDelta)
{
    m_pStateMachineCom->Upadte(fTimeDelta);

    Check_Navigation(m_pNavigationCom, m_pRootBoneMatrix);

    m_pNavigationCom->Update_WorldMatrix(XMMatrixIdentity());

    m_pHitBoxCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    m_pColliderCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    if (isCollUpdate)
        m_pGameInstance->Check_Collider(m_pHitBoxCom, TEXT("Player"));

    __super::Update(fTimeDelta);
}

void CKamen::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CKamen::Render()
{
#ifdef _DEBUG
    if (isCollUpdate)
        m_pHitBoxCom->Render();

    /* TEST */
   /*ImGui::Begin("Collider");
   ImGui::SliderFloat3("HitPos", reinterpret_cast<_float*>(&m_vHitBoxCenter), -7.f, 7.f);
   ImGui::SliderFloat3("HitExtents", reinterpret_cast<_float*>(&m_vHitBoxExtents), 0.3f, 7.f);
   ImGui::End();
   m_pHitBoxCom->Set_ColliderDesc(m_vHitBoxCenter, m_vHitBoxExtents);*/
#endif
    //m_pColliderCom->Render();
    return S_OK;
}

void CKamen::OnHit(const ATTACK_DESC& Attack_Desc)
{
    m_EnemyInfo.fHp -= Attack_Desc.fDamage;
  
}

void CKamen::Update_HitBox(_uint iSkillID, _uint iHitIndex)
{
    isCollUpdate = true;
    m_iCurSkillID = iSkillID;
    m_iCurHitIndex = iHitIndex;

    m_pGameInstance->Add_Collider(TEXT("Kamen_HitBox"), m_pHitBoxCom);
}

_bool CKamen::MoveToPlayer(_float fTimeDelta)
{
   return m_pTransformCom->MoveTo(fTimeDelta, m_pPlayerTransformCom->Get_Position(), 3.f, m_pNavigationCom);
}

void CKamen::Reposition()
{
    if(15.f <= XMVectorGetX(XMVector3Length(XMVectorSet(35.f, 0.1f, 50.f, 1.f) - m_pTransformCom->Get_Position())))
        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 50.f, 1.f));
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

    /* Collider */
    CBounding_OBB::BOUNDING_OBB_DESC OBB_Desc = {};
    OBB_Desc.vCenter = _float3(0.f, 0.5f, -1.41f);
    OBB_Desc.vExtents = _float3(0.5f, 0.3f, 1.31f);
    OBB_Desc.vOrientation = _float3(0.f, 0.f, 0.f);
    OBB_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBB_Desc)))
        return E_FAIL;

    /* Collider */
    OBB_Desc.vCenter = _float3(0.f, 0.5f, 0.f);
    OBB_Desc.vExtents = _float3(0.3f, 0.5f, 0.3f);
    OBB_Desc.vOrientation = _float3(0.f, 0.f, 0.f);
    OBB_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_HitBox_OBB"), reinterpret_cast<CComponent**>(&m_pHitBoxCom), &OBB_Desc)))
        return E_FAIL;

    m_pHitBoxCom->Set_OnCollisionEnter([&]() {
        deque<CGameObject*>& Objects = m_pHitBoxCom->Get_HitObjects();

        MONSTER_SKILL_INFO* pSkill = m_pGameManager->Get_KamenData(ENUM_TO_INT(m_ePhase), m_iCurSkillID);

        ATTACK_DESC Desc = {};
        Desc.eHitType = pSkill->eHitType;
        Desc.eAttackType = ATTACK_TYPE::NORMAL;
        _float fDamage = Desc.fDamage = pSkill->Damages[m_iCurHitIndex];

        while (!Objects.empty())
        {
            _float3 vPosition;
            XMStoreFloat3(&vPosition, Objects.front()->Get_Transform()->Get_Position());

            m_pGameManager->Add_DamageFont(DAMAGEFONT::PLAYER_HURT, fDamage, vPosition);
            dynamic_cast<CPlayer*>(Objects.front())->OnHit(Desc);
            Objects.pop_front();
        }
        });

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
    m_States[ENUM_TO_INT(KAMENSTATE::ATTACK_SWORD)] = CAttack_Sword_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::ATTACK_SPIN)] = CAttack_Spin_Kamen::Create(&Desc);


    return S_OK;
}

HRESULT CKamen::Ready_PartObjects()
{
    CPartObject::PARTOBJECT_DESC Body_Desc = {};
    Body_Desc.pParentTransform = m_pTransformCom;
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), 
        TEXT("Prototype_GameObject_Body_Kamen"), TEXT("Body_Kamen"), &Body_Desc)))
        return E_FAIL;

    m_pRootBoneMatrix = dynamic_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_root");

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

    for (_uint i = 0; i < ENUM_TO_INT(KAMENSTATE::END); i++)
    {
        Safe_Release(m_States[i]);
    }

    Safe_Release(m_pStateMachineCom);
    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pPlayerTransformCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pHitBoxCom);
}
