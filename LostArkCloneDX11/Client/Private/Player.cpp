#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

#include "PartObject.h"
#include "Body_Player.h"
#include "Weapon_Player.h"

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

    return m_pTransformCom->MoveTo(fTimeDelta, XMVectorSetW(XMLoadFloat3(m_pGameManager->Get_PickingPos()), 1.f));
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
    m_PlayerInfo.eStance = STANCE::FLURRY;
    m_PlayerInfo.fHp = m_PlayerInfo.fMaxHp = 10000.f;
    m_PlayerInfo.fMp = m_PlayerInfo.fMaxMp = 8000.f;
    m_PlayerInfo.fIdentity = 0.f;
    m_PlayerInfo.fAttack = 10000.f;

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

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
        m_pGameManager->Picking_Terrains();
}

void CPlayer::Update(_float fTimeDelta)
{
    if (MAX_IDENTITY >= m_PlayerInfo.fIdentity)
    {
        m_PlayerInfo.fIdentity += 2.f * fTimeDelta;
    }
    //if(m_pGameInstance->Get_KeyDown(DIK_Z))
    //    Change_Stance();

    __super::Update(fTimeDelta);

    m_pStateMachineCom->Upadte(fTimeDelta);

    m_pGameManager->Update_Skills(fTimeDelta);

    //Key_Input(fTimeDelta);

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
    CPartObject::PARTOBJECT_DESC Body_Desc = {};
    Body_Desc.pParentTransform = m_pTransformCom;

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
void CPlayer::Key_Input(_float fTimeDelta)
{
 /*   if(STATE::IDLE == m_eCurState)
    {
        if (m_pGameInstance->Get_KeyDown(DIK_D))
        {
            if (m_pGameManger->Use_Skill(6))
            {
                m_eCurState = STATE::ATTACK;
                m_iSkillID = 30;
                m_bSkillLoop = false;
            }
        }
        if (m_pGameInstance->Get_KeyDown(DIK_E))
        {
            if (m_pGameManger->Use_Skill(2))
            {
                m_eCurState = STATE::ATTACK;
                m_iSkillID = 137;
                m_bSkillLoop = false;
            }
        }
        else if (m_pGameInstance->Get_KeyDown(DIK_Z))
        {
            if (m_PlayerInfo.fIdentity > 5.f)
            {
                Change_Stance();
            }
        }
        else if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
        {
            m_pPickingPos = m_pGameManger->Get_PickingPos();
            if(nullptr != m_pPickingPos)
                m_eCurState = STATE::MOVE;
        }
    }*/
    
}

void CPlayer::Change_Stance()
{
    m_PlayerInfo.eStance = STANCE::FLURRY == m_PlayerInfo.eStance ? STANCE::FOCUS : STANCE::FLURRY;

    if (40.f <= m_PlayerInfo.fIdentity)  //น๖วม
        m_PlayerInfo.fIdentity -= 40.f;
    else
        m_PlayerInfo.fIdentity = 0.f;
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
