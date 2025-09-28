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
#include "Turn_Kamen.h"
#include "Clash_Kamen.h"
#include "Critical_Kamen.h"
#include "Move_Kamen.h"

#include "Body_Kamen.h"
#include "Weapon_Kamen.h"
#include "Player.h"
#include "Camera.h"

#include "ClashUI.h"

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

void	CKamen::Set_HitBox(_float3& vCenter, _float3& vExtends, COLLIDER eHitboxType)
{
    if(COLLIDER::OBB == eHitboxType)
        m_pHitBoxCom->Set_ColliderDesc(vCenter, vExtends);
    else
        m_pHitBoxShpereCom->Set_ColliderDesc(vCenter, vExtends);
}

void CKamen::Change_Phase(PHASE ePhase)
{
    m_ePhase = ePhase;

    _float3 vCenter = {};
    _float3 vExtend = {};

    CAttack_Kamen::ATTACK_KAMEN_DESC Desc = {};
    CNavigation::NAVIGATION_DESC NavDesc = {};

    NavDesc.iCurrentIndex = 1;

    switch (ePhase)
    {
    case Client::PHASE::PHASE1:
        // BossUI
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossUI"),
            ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_BossUI"))))
            return;

        m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_Fovy(60.f);
        m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_LookDircetion(XMVectorSet(0.f, 5.f, -5.f, 0.f));

        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 50.f, 1.f));
        m_pStateMachineCom->Change_State(Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
        static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Set_Animation(59, false, 0.2f);
        break;

    case Client::PHASE::PHASE2:
            static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Change_Model(PHASE::PHASE2);

            m_pRootBoneMatrix = static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_root");

            static_cast<CWeapon_Kamen*>(Find_PartObject(TEXT("Weapon_Kamen")))->Change_SocketMatrix(
                dynamic_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_wpn_02"));

            vCenter = _float3(0.f, 0.f, -1.5f);
            vExtend = _float3(1.2f, 1.72f, 1.5f);
            m_pColliderCom->Set_ColliderDesc(vCenter, vExtend);

            m_pStateMachineCom->Change_State(Get_State(CKamen::KAMENSTATE::INTRO), nullptr);
            m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 60.f, 1.f));
        break;

    case Client::PHASE::PHASE3:
        dynamic_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Change_Model(PHASE::PHASE3);

        m_pRootBoneMatrix = static_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_root");

        static_cast<CWeapon_Kamen*>(Find_PartObject(TEXT("Weapon_Kamen")))->Change_SocketMatrix(
            dynamic_cast<CBody_Kamen*>(Find_PartObject(TEXT("Body_Kamen")))->Get_BoneMatrixPtr("b_wp_1"));
       
        if (FAILED(__super::Change_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_KamenPhase2"),
            TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavDesc)))
            return;

        vCenter = _float3(0.f, 0.f, -1.0f);
        vExtend = _float3(0.7f, 0.7f, 1.f);
        m_pColliderCom->Set_ColliderDesc(vCenter, vExtend);

        m_pStateMachineCom->Change_State(Get_State(CKamen::KAMENSTATE::INTRO), nullptr);
        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(30.f, 0.f, 40.f, 1.f));

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


    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (nullptr == m_pPlayerTransformCom)
        return E_FAIL;

    Safe_AddRef(m_pPlayerTransformCom);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Reay_Component()))
        return E_FAIL;

    if (FAILED(Reay_States()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_ePhase = PHASE::INTRO;

    m_EnemyInfo.fAttack = 200.f;
    m_EnemyInfo.fAttackRange = 3.f;
    m_EnemyInfo.fDetectDistance = 5.f;
    m_EnemyInfo.fHp = m_EnemyInfo.fMaxHp = 1000000.f;


    m_pStateMachineCom->Start_State(m_States[ENUM_TO_INT(KAMENSTATE::INTRO)]);

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 2.9f, 71.f, 1.f));
    m_pTransformCom->Rotation(0.f, XMConvertToRadians(180.f), 0.f);

    return S_OK;
}

void CKamen::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    isCollUpdate = false;
    isSphereUpdate = false;

    m_pGameInstance->Add_Collider(TEXT("Monster"), m_pColliderCom);

    if (PHASE::PHASE1 == m_ePhase && 0.5f >= m_EnemyInfo.fHp / m_EnemyInfo.fMaxHp)
        Change_Phase(PHASE::PHASE2);
}

void CKamen::Update(_float fTimeDelta)
{
    m_pStateMachineCom->Upadte(fTimeDelta);

    if(PHASE::INTRO != m_ePhase)
        Check_Navigation(m_pNavigationCom, m_pRootBoneMatrix);

    m_pNavigationCom->Update_WorldMatrix(XMMatrixIdentity());

    m_pHitBoxCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    m_pHitBoxShpereCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    m_pColliderCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    if (isCollUpdate)
        m_pGameInstance->Check_Collider(m_pHitBoxCom, TEXT("Player"));
    if(isSphereUpdate)
        m_pGameInstance->Check_Collider(m_pHitBoxShpereCom, TEXT("Player"));

    __super::Update(fTimeDelta);
}

void CKamen::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    if (m_pGameInstance->Get_KeyDown(DIK_5))
    {
        Change_Phase(PHASE::PHASE3);
    }

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CKamen::Render()
{
#ifdef _DEBUG
    if (isCollUpdate)
        m_pHitBoxCom->Render();
   
    if(isSphereUpdate)
        m_pHitBoxShpereCom->Render();

    _float dis = XMVector3Length(XMVectorSetY(m_pTransformCom->Get_Position(), 0.f) - XMVectorSetY(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_Position(), 0.f)).m128_f32[0];

    ImGui::SliderFloat("dis", &dis, 0.1f, 1.f);
   // /* TEST */
  /* ImGui::Begin("Collider");
   ImGui::SliderFloat3("HitPos", reinterpret_cast<_float*>(&m_vHitBoxCenter), -7.f, 7.f);
   ImGui::SliderFloat3("HitExtents", reinterpret_cast<_float*>(&m_vHitBoxExtents), 0.3f, 15.f);
   ImGui::End();
   m_pColliderCom->Set_ColliderDesc(m_vHitBoxCenter, m_vHitBoxExtents);*/
#endif
   m_pColliderCom->Render();
    return S_OK;
}

void CKamen::OnHit(const ATTACK_DESC& Attack_Desc)
{
    m_EnemyInfo.fHp -= Attack_Desc.fDamage;
  
}

void CKamen::Update_HitBox(_uint iSkillID, _uint iHitIndex, COLLIDER eHitboxType)
{
    m_iCurSkillID = iSkillID;
    m_iCurHitIndex = iHitIndex;

    if(COLLIDER::OBB == eHitboxType)
    {
        isCollUpdate = true;
        m_pGameInstance->Add_Collider(TEXT("Kamen_HitBox"), m_pHitBoxCom);
    }
    else
    {
        isSphereUpdate = true;
        m_pGameInstance->Add_Collider(TEXT("Kamen_HitBox"), m_pHitBoxShpereCom);
    }

}

_bool CKamen::MoveToPlayer(_float fTimeDelta)
{
   return m_pTransformCom->MoveTo(fTimeDelta, m_pPlayerTransformCom->Get_Position(), 3.f, m_pNavigationCom);
}

_bool CKamen::Reposition()
{
    if(15.f <= XMVectorGetX(XMVector3Length(XMVectorSet(35.f, 0.1f, 50.f, 1.f) - m_pTransformCom->Get_Position())))
    {
        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 50.f, 1.f));
        return true;
    }

    return false;
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

    /* Collider */CBounding_OBB::BOUNDING_OBB_DESC ColliderDesc = {};
    ColliderDesc.eColliderType = COLLIDERTYPE::COLLIDER;
    ColliderDesc.vCenter = _float3(0.f, 0.0f, -1.f);
    ColliderDesc.vExtents = _float3(0.7f, 0.7f, 1.0f);
    ColliderDesc.vOrientation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    /* Hitbox OBB */
    CBounding_OBB::BOUNDING_OBB_DESC OBB_Desc = {};
    OBB_Desc.eColliderType = COLLIDERTYPE::HITBOX;
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
        _float fDamage = Desc.fDamage = pSkill->HitBoxDescs[m_iCurHitIndex].fDamage;

        while (!Objects.empty())
        {
            _float3 vPosition;
            XMStoreFloat3(&vPosition, Objects.front()->Get_Transform()->Get_Position());

            m_pGameManager->Add_DamageFont(DAMAGEFONT::PLAYER_HURT, fDamage, vPosition);
            dynamic_cast<CPlayer*>(Objects.front())->OnHit(Desc);
            Objects.pop_front();
        }
        });


    /* Hitbox Sphere */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
    SphereDesc.eColliderType = COLLIDERTYPE::HITBOX;
    SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
    SphereDesc.fRadius = 3.f;
    SphereDesc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_HitBox_Sphere"), reinterpret_cast<CComponent**>(&m_pHitBoxShpereCom), &SphereDesc)))
        return E_FAIL;

    m_pHitBoxShpereCom->Set_OnCollisionEnter([&]() {
        deque<CGameObject*>& Objects = m_pHitBoxShpereCom->Get_HitObjects();

        MONSTER_SKILL_INFO* pSkill = m_pGameManager->Get_KamenData(ENUM_TO_INT(m_ePhase), m_iCurSkillID);

        ATTACK_DESC Desc = {};
        Desc.eHitType = pSkill->eHitType;
        Desc.eAttackType = ATTACK_TYPE::NORMAL;
        _float fDamage = Desc.fDamage = pSkill->HitBoxDescs[m_iCurHitIndex].fDamage;


        if (pSkill->HitBoxDescs[m_iCurHitIndex].vExtends.y >= XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - m_pPlayerTransformCom->Get_Position())))
            return;

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
    m_States[ENUM_TO_INT(KAMENSTATE::TRUN)] = CTurn_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::CUTSCENE)] = CTurn_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::CLASH)] = CClash_Kamen::Create(&Desc, m_pPlayerTransformCom);
    m_States[ENUM_TO_INT(KAMENSTATE::CRITICAL)] = CCritical_Kamen::Create(&Desc);
    m_States[ENUM_TO_INT(KAMENSTATE::MOVE)] = CMove_Kamen::Create(&Desc);

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
    Safe_Release(m_pHitBoxShpereCom);
}
