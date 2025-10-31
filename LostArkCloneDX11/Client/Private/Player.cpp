#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

#include "PartObject.h"
#include "Body_Player.h"
#include "Weapon_Player.h"
#include "WeaponEffect_Player.h"
#include "HpBar_Player.h"
#include "Test_Effect.h"
#include "Buff.h"
#include "Camera_Fix.h"
#include "Enemy.h"
#include "Terrain.h"
#include "MapObject.h"
#include "Effect_Trail.h"

#pragma region STATE
#include "StateMachine.h"
#include "State.h"
#include "Player_Idle.h"
#include "Player_Move.h"
#include "Player_NormalSkill.h"
#include "Player_ChargeSkill.h"
#include "Player_ComboSkill.h"
#include "Player_AwakeSkill.h"
#include "Player_ChangeStance.h"
#include "Player_Dash.h"
#include "Player_Hit.h"
#include "Player_CutScene.h"
#include "Player_Clash.h"
#include "Player_Dialogue.h"
#pragma endregion

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCharacter{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CCharacter{ Prototype }
{
}

_float CPlayer::Get_TrackPositon()
{
    return static_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_TrackPoisiton();
}

CPartObject* CPlayer::Get_PartObject(const _tchar* PartObjectTag)
{
    return Find_PartObject(PartObjectTag);
}

void CPlayer::Set_State(STATE eState, void* pArg)
{
    m_pStateMachineCom->Change_State(m_States[eState], pArg);
}

void CPlayer::Set_ChargeSkill_Desc(_bool isUsing, _float fChargingTime)
{
    m_ChargeSkill_Desc.isUsing = isUsing;
    m_ChargeSkill_Desc.fChargingTime = fChargingTime;
}

void CPlayer::Set_Animation(_uint iIndex, _bool bLoop, _float fLerpTime)
{
    static_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Set_Animation(iIndex, bLoop, fLerpTime);
}

void CPlayer::Set_HitBox(_float3& vCenter, _float3& vExtends, _float3& vOrientation, COLLIDER eType)
{
    if(COLLIDER::OBB == eType)
        m_pHitBoxCom->Set_ColliderDesc(vCenter, vExtends, vOrientation);
    else if(COLLIDER::SPHERE == eType)
    {
        m_pHitSphereCom->Set_ColliderDesc(vCenter, vExtends);
        m_pHitSphereCom->Update(XMMatrixTranslationFromVector(XMVectorSetW(XMLoadFloat3(&vOrientation), 1.f)));
    }
}

_bool CPlayer::isAnimationFinish()
{
    return static_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->isAnimationFinish();
}

HRESULT CPlayer::Change_Level(_fvector vPositon, const _tchar* pNavigationPrototypeTag)
{
    m_pTransformCom->Set_State(Engine::STATE::POSITION, vPositon);

    CNavigation::NAVIGATION_DESC NavDesc = {};

    NavDesc.iCurrentIndex = 1;

    if (FAILED(__super::Change_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), pNavigationPrototypeTag,
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavDesc)))
        return E_FAIL;

    return S_OK;
}

void CPlayer::EnterBoss()
{
    m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_Fovy(40.f);
    m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_LookDircetion(XMVectorSet(0.f, 3.2f, -13.3f, 0.f));

    m_pNavigationCom->Set_Current_CellIndex(0);
    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 32.f, 1.f));
    m_pStateMachineCom->Change_State(m_States[ENUM_TO_INT(STATE::CUTSCENE)], nullptr);
}


void CPlayer::Start_Phase(_uint iPhaseIndex)
{
    if(3 == iPhaseIndex)
    {
        if (FAILED(m_pGameInstance->Clear_Layer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Terrain"))))
            return;

        if (FAILED(m_pGameInstance->Clear_Layer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Background"))))
            return;

        if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/MapData/Kamen_Phase2.xml")))
            return;

        for (const auto& TerrainData : CGameManager::GetInstance()->Get_TerrainData())
        {
            CTerrain::TERRAIN_DESC Desc = {};

            Desc.strPrototypeTag = TerrainData.strPrototypeTag;
            Desc.vPosition = TerrainData.vPosition;
            Desc.vRotation = TerrainData.vRotation;

            if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
                ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Terrain"), &Desc)))
                return;
        }

        for (auto& MapData : CGameManager::GetInstance()->Get_MapData())
        {
            CMapObject::MAPOBJECT_DESC Desc = {};

            Desc.iLevelIndex = ENUM_TO_INT(LEVEL::BOSS);
            Desc.strPrototypeTag = MapData.strPrototypeTag;
            Desc.vPosition = MapData.vPosition;
            Desc.vRotation = MapData.vRotation;
            Desc.vScale = MapData.vScale;

            if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
                ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Background"), &Desc)))
                return;
        }

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Kamen_Area"),
            ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_KamenArea"), nullptr)))
            return;



        CNavigation::NAVIGATION_DESC NavDesc = {};

        NavDesc.iCurrentIndex = 1;

        if (FAILED(__super::Change_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_KamenPhase2"),
            TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavDesc)))
            return;

        m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(30.f, 0.f, 30.f, 1.f));
    }
}

void CPlayer::Toggle_PartObject(const _tchar* PartObjectTag)
{
    Find_PartObject(PartObjectTag)->Toggle_Visible();
}

_bool CPlayer::Move(_float fTimeDelta)
{
    _vector vPositon = m_pGameManager->Get_PickingPos();

    if (true == m_isColl || 0.f >= XMVectorGetX(vPositon))
        return false;

    return m_pTransformCom->MoveTo(fTimeDelta, vPositon, m_PlayerInfo.fMoveSpeed, m_pNavigationCom);
}

void CPlayer::TurnToCursor()
{
    m_pTransformCom->TurnTo(m_pGameManager->Picking_Terrains());
}

HRESULT CPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    m_DefaultInfo.eStance = STANCE::FLURRY;
    m_DefaultInfo.fHp = m_DefaultInfo.fMaxHp = 50000.f;
    m_DefaultInfo.fMp = m_DefaultInfo.fMaxMp = 8000.f;
    m_DefaultInfo.fIdentity = 80.f;
    m_DefaultInfo.fAttack = 10000.f;
    m_DefaultInfo.Critical_Probability = 20.f;
    m_DefaultInfo.Critical_Damage = 1.5f;
    m_DefaultInfo.fAttackSpeed = 1.f;
    m_DefaultInfo.fMoveSpeed = 3.f;

    memcpy(&m_PlayerInfo, &m_DefaultInfo, sizeof(PLAYER_INFO));

    m_ChargeSkill_Desc.isUsing = false;
    m_ChargeSkill_Desc.fMaxChargeTime = 1.f;
    m_ChargeSkill_Desc.fChargingTime = 0.f;


    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 5.f;
    Desc.fSpeedPersec = 3.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    if (FAILED(Ready_StateMachine()))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(45.f, 0.f, 45.f, 1.f));

    m_pStateMachineCom->Start_State(m_States[IDLE]);

    m_pGameManager->Set_PlayerInfoPrt(&m_PlayerInfo);

    m_pRootBoneMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_root");

    Add_Buff(1);


#ifdef _DEBUG
    //m_pGameInstance->Bind_Camera(TEXT("Camera_Free"));
#endif

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
        m_pGameManager->Picking_Terrains();

    Update_Buff(fTimeDelta);
    
    m_pGameInstance->Add_Collider(TEXT("Player"), m_pColliderCom);

#ifdef _DEBUG
    isCollUpdate = false;
    isHitSphereUpdate = false;




#endif // _DEBUG
}

void CPlayer::Update(_float fTimeDelta)
{
    if (MAX_IDENTITY >= m_PlayerInfo.fIdentity)
    {
        m_PlayerInfo.fIdentity += 2.f * fTimeDelta;
    }
    if (m_pGameInstance->Get_KeyDown(DIK_1))
    {
        m_PlayerInfo.fHp = max(m_PlayerInfo.fMaxHp, m_PlayerInfo.fHp + m_PlayerInfo.fMaxHp * 0.3f);
    }
    if (m_PlayerInfo.fMaxMp < m_PlayerInfo.fMp)
    {
        m_PlayerInfo.fMp = m_PlayerInfo.fMaxMp;
    }
    else
    {
        m_PlayerInfo.fMp += fTimeDelta * 150.f;
    }

    __super::Update(fTimeDelta);

    m_pStateMachineCom->Upadte(fTimeDelta);

    m_pGameManager->Update_Skills(fTimeDelta);

    m_pNavigationCom->Update_WorldMatrix(XMMatrixIdentity());

    m_pHitBoxCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
   
    m_pColliderCom->Update(XMLoadFloat4x4(m_pRootBoneMatrix) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    if (isCollUpdate)
        m_pGameInstance->Check_Collider(m_pHitBoxCom, TEXT("Monster"));

    if (isHitSphereUpdate)
        m_pGameInstance->Check_Collider(m_pHitSphereCom, TEXT("Monster"));

   // m_isColl = m_pGameInstance->Check_Collider(m_pColliderCom, TEXT("Monster"));
    //
    //if(true == m_isColl)
    //{
    //    _vector vDirection = m_pGameInstance->ComputePenetration(m_pColliderCom, TEXT("Monster"));
        //m_pTransformCom->Go_Backward_World(fTimeDelta);
        //m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pTransformCom->Get_Position() + vDirection);
    //}


}

void CPlayer::Late_Update(_float fTimeDelta)
{
    /* TEST */
    __super::Late_Update(fTimeDelta);
 
    m_pColliderCom->Update_OnCollision();

}

HRESULT CPlayer::Render()
{
 
  

    return S_OK;
}


void CPlayer::OnHit(const ATTACK_DESC& Attack_Desc)
{
    if (m_iCurSkillID == 99)
    {
        m_PlayerInfo.fHp -= Attack_Desc.fDamage;

        CPlayer_Hit::PLAYER_HIT_DESC Desc = {};
        Desc.eType = Attack_Desc.eHitType;
    
        XMStoreFloat3(&Desc.vPosition, m_pColliderCom->Get_HitBoxDesc().pHitObject->Get_Transform()->Get_Position());

        m_pStateMachineCom->Change_State(m_States[STATE::HIT], &Desc);
    }
    else 
    {
        if (true == m_pGameManager->Get_SkillInfo_Prt(m_iCurSkillID)->bInvincible)
            return;

        if(8 == m_iCurSkillID)
        {
            m_PlayerInfo.fHp -= Attack_Desc.fDamage * 0.1f;

            CPlayer_NormalSkill::NORMALSKILL_DESC Desc = {};

            Desc.bLoop = false;
            Desc.iAnimationIndex = 59;
            Desc.iSkillID = 9;

            m_pStateMachineCom->Change_State(Get_State(CPlayer::STATE::NORMAL_SKILL), &Desc);

        }
        else
            m_PlayerInfo.fHp -= Attack_Desc.fDamage;

        if (HIT_TYPE::PUSH == Attack_Desc.eHitType &&
            m_pGameManager->Get_SkillInfo_Prt(m_iCurSkillID)->eSuperArmour & SUPER_ARMOUR::SUPRE_ARMOUR_PUSH)
        {
            return;
        }
        else if (HIT_TYPE::NORMAL == Attack_Desc.eHitType &&
            m_pGameManager->Get_SkillInfo_Prt(m_iCurSkillID)->eSuperArmour & SUPER_ARMOUR::SUPRE_ARMOUR_PARALYSIS)
        {
            return;
        }

        CPlayer_Hit::PLAYER_HIT_DESC Desc = {};
        Desc.eType = Attack_Desc.eHitType;


        CGameObject* pObject = m_pColliderCom->Get_HitBoxDesc().pHitObject;
        if (nullptr != pObject)
        {
            XMStoreFloat3(&Desc.vPosition, pObject->Get_Transform()->Get_Position());
            m_pStateMachineCom->Change_State(m_States[STATE::HIT], &Desc);
        }
    }
    
}

HRESULT CPlayer::Ready_Components()
{
    CNavigation::NAVIGATION_DESC NavDesc = {};

    NavDesc.iCurrentIndex = 0;

    /* Navigation */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Trision"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
        return E_FAIL;

    /* HITBOX */
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

        if (99 == m_iCurSkillID)
            return;

        SKILL_INFO* pSkill = m_pGameManager->Get_SkillInfo_Prt(m_iCurSkillID);

        ATTACK_DESC Desc = {};
        Desc.eAttackType = pSkill->eAttackType;
        Desc.eHitType = pSkill->eHitType;

        _float fDamage = pSkill->Damages[m_iCurHitIndex];

        m_PlayerInfo.fIdentity = min(MAX_IDENTITY, m_PlayerInfo.fIdentity + 30.f);

        while (!Objects.empty())
        {
            if (m_PlayerInfo.Critical_Probability > m_pGameInstance->Random_Normal())
            {
                Desc.isCritial = true;
                Desc.fDamage = m_PlayerInfo.Critical_Damage * (fDamage + m_PlayerInfo.fAttack);

            }
            else
            {
                Desc.isCritial = false;
                Desc.fDamage = fDamage + m_PlayerInfo.fAttack;
            }
            DAMAGEFONT eDamageType = Desc.isCritial == true ? DAMAGEFONT::CRITICAL : DAMAGEFONT::NORMAL;
            _float fFontDamage = Desc.fDamage;
            _float3 vPosition;
            XMStoreFloat3(&vPosition, Objects.front()->Get_Transform()->Get_Position());

            _uint iEffectID = m_PlayerInfo.eStance == STANCE::FLURRY ? 13 : 12;

            m_pGameManager->Add_Effect(EFFECT::GROUND, iEffectID, &Objects.front()->Get_Transform()->Get_WorldMatrix());
            m_pGameManager->Add_Effect(EFFECT::PARTICLE, 11, &Objects.front()->Get_Transform()->Get_WorldMatrix());

            m_pGameManager->Add_DamageFont(eDamageType, fFontDamage, vPosition);
            dynamic_cast<CEnemy*>(Objects.front())->OnHit(Desc);
            Objects.pop_front();
        }
        
        });

    /* HITBOX */
    CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
    Sphere_Desc.eColliderType = COLLIDERTYPE::HITBOX;
    Sphere_Desc.vCenter = _float3(0.f, 0.5f, 0.f);
    Sphere_Desc.fRadius = 1.f;
    Sphere_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_HitBox_Sphere"), reinterpret_cast<CComponent**>(&m_pHitSphereCom), &Sphere_Desc)))
        return E_FAIL;


    m_pHitSphereCom->Set_OnCollisionEnter([&]() {

        deque<CGameObject*>& Objects = m_pHitSphereCom->Get_HitObjects();

        if (99 == m_iCurSkillID)
            return;

        SKILL_INFO* pSkill = m_pGameManager->Get_SkillInfo_Prt(m_iCurSkillID);

        ATTACK_DESC Desc = {};
        Desc.eAttackType = pSkill->eAttackType;
        Desc.eHitType = pSkill->eHitType;

        _float fDamage = pSkill->Damages[m_iCurHitIndex];

        m_PlayerInfo.fIdentity = min(MAX_IDENTITY, m_PlayerInfo.fIdentity + 30.f);

        while (!Objects.empty())
        {
            if (m_PlayerInfo.Critical_Probability > m_pGameInstance->Random_Normal())
            {
                Desc.isCritial = true;
                Desc.fDamage = m_PlayerInfo.Critical_Damage * (fDamage + m_PlayerInfo.fAttack);

            }
            else
            {
                Desc.isCritial = false;
                Desc.fDamage = fDamage + m_PlayerInfo.fAttack;
            }
            DAMAGEFONT eDamageType = Desc.isCritial == true ? DAMAGEFONT::CRITICAL : DAMAGEFONT::NORMAL;
            _float fFontDamage = Desc.fDamage;
            _float3 vPosition;
            XMStoreFloat3(&vPosition, Objects.front()->Get_Transform()->Get_Position());

            _uint iEffectID = m_PlayerInfo.eStance == STANCE::FLURRY ? 13 : 12;

            m_pGameManager->Add_Effect(EFFECT::GROUND, iEffectID, &Objects.front()->Get_Transform()->Get_WorldMatrix());
            m_pGameManager->Add_Effect(EFFECT::PARTICLE, 11, &Objects.front()->Get_Transform()->Get_WorldMatrix());

            m_pGameManager->Add_DamageFont(eDamageType, fFontDamage, vPosition);
            dynamic_cast<CEnemy*>(Objects.front())->OnHit(Desc);
            Objects.pop_front();
        }

        });

    /* Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC ColliderDesc = {};
    ColliderDesc.eColliderType = COLLIDERTYPE::COLLIDER;
    ColliderDesc.vCenter = _float3(0.f, 0.5f, 0.f);
    ColliderDesc.fRadius = 0.4f;
    ColliderDesc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
    CBody_Player::BODYPLAYER_DESC Body_Desc = {};
    Body_Desc.pStance = &m_PlayerInfo.eStance;
    Body_Desc.pParentTransform = m_pTransformCom;
    Body_Desc.pAttackSpeed = &m_PlayerInfo.fAttackSpeed;
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), TEXT("Body_Player"), &Body_Desc)))
        return E_FAIL;

    CWeapon_Player::WEAPON_DESC Weapon_Desc = {};
    Weapon_Desc.pStance = &m_PlayerInfo.eStance;
    Weapon_Desc.pParentTransform = m_pTransformCom;
    Weapon_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");

    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Weapon_Player"), TEXT("Weapon_Player"), &Weapon_Desc)))
        return E_FAIL;

    CWeaponEffect_Player::WEAPON_DESC WeaponEffect_Desc = {};
    WeaponEffect_Desc.pParentTransform = m_pTransformCom;
    WeaponEffect_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");

    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WeaponEffect_Player"), TEXT("WeaponEffect_Player"), &WeaponEffect_Desc)))
        return E_FAIL;

    CHpBar_Player::HPBARPLAYER_DESC  HpBar_Desc= {};
    HpBar_Desc.pPlayerInfo = &m_PlayerInfo;
    HpBar_Desc.pParentTransform = m_pTransformCom;
    HpBar_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_effectname");
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpBar_Player"), TEXT("HPBar_Player"), &HpBar_Desc)))
        return E_FAIL;

 CEffect::EFFECT_DESC Effect_Desc= {};
     
    //Effect_Desc.pParentTransform = m_pTransformCom;
    //Effect_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");
    //if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Test_Effect"), TEXT("Test_Effect"), &Effect_Desc)))
    //    return E_FAIL;

    // 
    //Effect_Desc.pParentTransform = m_pTransformCom;
    //Effect_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");
    //if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Test_MeshEffect"), TEXT("Test_Effect"), &Effect_Desc)))
    //    return E_FAIL;

 
    //Effect_Desc.pParentTransform = m_pTransformCom;
    //Effect_Desc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");
    //if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Test_Ground_Effect"), TEXT("Ground_Effect"), &Effect_Desc)))
    //    return E_FAIL;

   /* CEffect_Trail::TRAIL_EFFECT_DESC TrailDesc = {};

    TrailDesc.pParentTransform = m_pTransformCom;
    TrailDesc.pSocketMatrix = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->Get_BoneMatrixPtr("b_weapon_rhand");
    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Trail"), TEXT("Trail_Effect"), &TrailDesc)))
        return E_FAIL;*/

    return S_OK;
}

HRESULT CPlayer::Ready_StateMachine()
{
    /* StateMachine */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
        TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Ready_States()
{
    m_States[IDLE] = CPlayer_Idle::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[MOVE] = CPlayer_Move::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[NORMAL_SKILL] = CPlayer_NormalSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CHARGE_SKILL] = CPlayer_ChargeSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[COMBO_SKILL] = CPlayer_ComboSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[DASH] = CPlayer_Dash::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CHANGE_STANCE] = CPlayer_ChangeStance::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[HIT] = CPlayer_Hit::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CUTSCENE] = CPlayer_CutScene::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[CLASH] = CPlayer_Clash::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[DIALOGUE] = CPlayer_Dialogue::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);
    m_States[AWAKE_SKILL] = CPlayer_AwakeSkill::Create(m_pStateMachineCom, &m_PlayerInfo.eStance, this);

    return S_OK;
}

void CPlayer::Update_Buff(_float fTimeDelta)
{
    if (m_Buffs.empty())
    {
        m_PlayerInfo.Critical_Damage = m_DefaultInfo.Critical_Damage;
        m_PlayerInfo.Critical_Probability = m_DefaultInfo.Critical_Probability;
        m_PlayerInfo.fAttack = m_DefaultInfo.fAttack;
        m_PlayerInfo.fAttackSpeed = m_DefaultInfo.fAttackSpeed;
        m_PlayerInfo.fMoveSpeed = m_DefaultInfo.fMoveSpeed;
        return;
    }

    auto iter = m_Buffs.begin();

    ZeroMemory(&m_BuffStat, sizeof(BUFFSTAT));

    for (; iter != m_Buffs.end();)
    {
        (*iter)->Update(fTimeDelta, &m_BuffStat);

        if ((*iter)->isFinished())
        {
            m_pGameManager->Remove_Buff(*iter);
            iter = m_Buffs.erase(iter);
        }
        else
            iter++;
    }

    m_PlayerInfo.Critical_Damage = m_DefaultInfo.Critical_Damage * (1.f + m_BuffStat.Critical_Damage_Pct);
    m_PlayerInfo.Critical_Probability = m_DefaultInfo.Critical_Probability + m_BuffStat.Critical_Probability_Flat;
    m_PlayerInfo.fAttack = (m_DefaultInfo.fAttack + m_BuffStat.fAtk_Flat) * (1.f + m_BuffStat.Critical_Damage_Pct);
    m_PlayerInfo.fAttackSpeed = m_DefaultInfo.fAttackSpeed * (1.f + m_BuffStat.fAtkSpeed_Pct);
    m_PlayerInfo.fMoveSpeed = m_DefaultInfo.fMoveSpeed * (1.f + m_BuffStat.fMoveSpeed_Pct);
}

void CPlayer::Key_Input(_float fTimeDelta)
{
    
}

void CPlayer::Change_Stance()
{
    m_PlayerInfo.eStance = STANCE::FLURRY == m_PlayerInfo.eStance ? STANCE::FOCUS : STANCE::FLURRY;

    if (40.f <= m_PlayerInfo.fIdentity)  //น๖วม
    {
        m_PlayerInfo.fIdentity -= 40.f;
        Add_Buff(0);
    }
    else
        m_PlayerInfo.fIdentity = 0.f;
}

void CPlayer::Add_Buff(_uint iBuffID)
{
    auto iter = m_Buffs.begin();

    for (; iter != m_Buffs.end();)
    {

        if ((*iter)->Get_BuffID() == iBuffID)
        {
            m_pGameManager->Remove_Buff(*iter);
            iter = m_Buffs.erase(iter);
            break;
        }
        else
            iter++;
    }

    m_Buffs.push_back(m_pGameManager->Add_Buff(iBuffID));
}

void CPlayer::Update_HitBox(_uint iSkillID, _uint iHitIndex, COLLIDER eType)
{
    m_iCurSkillID = iSkillID;
    m_iCurHitIndex = iHitIndex;
  
    if(COLLIDER::OBB == eType)
    {
        isCollUpdate = true;
        m_pGameInstance->Add_Collider(TEXT("Player"), m_pHitBoxCom);
    }
    else
    {
        isHitSphereUpdate = true;
        m_pGameInstance->Add_Collider(TEXT("Player"), m_pHitSphereCom);
    }
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CPlayer");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CGameObject* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CPlayer");
        return nullptr;
    }

    return pInstance;
}

void CPlayer::Free()
{
    __super::Free();

    for (auto& pBuff : m_Buffs)
        Safe_Release(pBuff);

  
    for (_uint i = 0; i < STATE::STATE_END; i++)
    {
        Safe_Release(m_States[i]);
    }

    Safe_Release(m_pStateMachineCom);

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pHitBoxCom);
    Safe_Release(m_pHitSphereCom);
}
