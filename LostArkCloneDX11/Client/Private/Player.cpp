#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

#include "PartObject.h"
#include "Body_Player.h"
#include "Weapon_Player.h"
#include "Buff.h"
#include "Camera_Fix.h"

#pragma region STATE
#include "StateMachine.h"
#include "State.h"
#include "Player_Idle.h"
#include "Player_Move.h"
#include "Player_NormalSkill.h"
#include "Player_ChargeSkill.h"
#include "Player_ComboSkill.h"
#include "Player_ChangeStance.h"
#include "Player_Dash.h"
#include "Player_Hit.h"
#pragma endregion

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject{ pDevice, pContext }, m_pGameManager{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameManager);
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject{ Prototype },
    m_pGameManager{Prototype.m_pGameManager }
{
    Safe_AddRef(m_pGameManager);
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

_bool CPlayer::isAnimationFinish()
{
    return static_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")))->isAnimationFinish();
}

_bool CPlayer::Move(_float fTimeDelta)
{
    if (nullptr == m_pGameManager->Get_PickingPos())
        return false;

    return m_pTransformCom->MoveTo(fTimeDelta, XMVectorSetW(XMLoadFloat3(m_pGameManager->Get_PickingPos()), 1.f), m_PlayerInfo.fMoveSpeed);
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
    m_DefaultInfo.fHp = m_DefaultInfo.fMaxHp = 10000.f;
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

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    if (FAILED(Ready_StateMachine()))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(40.f, 0.f, 40.f, 1.f));

    m_pStateMachineCom->Start_State(m_States[IDLE]);

    m_pGameManager->Set_PlayerInfoPrt(&m_PlayerInfo);

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
        m_pGameManager->Picking_Terrains();

    Update_Buff(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    if (MAX_IDENTITY >= m_PlayerInfo.fIdentity)
    {
        m_PlayerInfo.fIdentity += 2.f * fTimeDelta;
    }

    __super::Update(fTimeDelta);

    m_pStateMachineCom->Upadte(fTimeDelta);

    m_pGameManager->Update_Skills(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);

    __super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{

    return S_OK;
}
HRESULT CPlayer::Ready_PartObjects()
{
    CBody_Player::BODYPLAYER_DESC Body_Desc = {};
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
    auto iter = find_if(m_Buffs.begin(), m_Buffs.end(), [&](CBuff* pBuff)->_bool {
        return (pBuff->Get_BuffID() == iBuffID);
        });

    if(m_Buffs.end() == iter)
        m_Buffs.push_back(m_pGameManager->Add_Buff(iBuffID));
}

void CPlayer::Play_CameraAnimation(CAMERA_ANIM eState)
{
    dynamic_cast<CCamera_Fix*>(CGameManager::GetInstance()->Get_Camera())->Set_State(eState);
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

    Safe_Release(m_pGameManager);
    Safe_Release(m_pStateMachineCom);

    for (_uint i = 0; i < STATE::STATE_END; ++i)
    {
        Safe_Release(m_States[i]);
    }
}
