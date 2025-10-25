#include "pch.h"
#include "Player_ChangeStance.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Weapon_Player.h"
#include "Body_Player.h"

CPlayer_ChangeStance::CPlayer_ChangeStance()
	:CSkill_Player{}
{
}

HRESULT CPlayer_ChangeStance::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_ChangeStance::Enter(void* pArg)
{
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(8);

	_uint iEffectID = {};

	if (STANCE::FLURRY == *m_pPlayerStance)
	{
		iEffectID = 19;
		m_pPlayer->Set_Animation(40, false);
		m_pGameInstance->Play_Sound(L"ToFlurry3.wav", CHANNELID::SKILL_PLAYER, 0.2f);
	}
	else
	{
		iEffectID = 20;
		m_pPlayer->Set_Animation(41, false);
		m_pGameInstance->Play_Sound(L"ToFocus3.wav", CHANNELID::SKILL_PLAYER, 0.2f);
	}

	m_pPlayer->Change_Stance();

	const vector<EFFECT_EVENT_DESC>& EffectEvents = m_pGameManager->Get_EffectTrack(CHARACTER::PLAYER, iEffectID);

	m_EffectEvents.reserve(EffectEvents.size());

	for (const auto& Track : EffectEvents)
	{
		m_EffectEvents.push_back({ false, Track });
	}

	dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();
}

void CPlayer_ChangeStance::Update(_float fTimeDelta)
{
	if (m_pPlayer->isAnimationFinish())
	{
		if(m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}

	for (auto& Event : m_EffectEvents)
	{
		if (false == Event.isTrigge)
		{
			if (m_pPlayer->Get_TrackPositon() >= Event.EventDesc.fKeyFrame)
			{
				Event.isTrigge = true;
				m_pGameManager->Add_Effect(Event.EventDesc.eType, Event.EventDesc.iID, m_pPlayerWorldMatrix, CHARACTER::PLAYER);
			}
		}
	}
}

void CPlayer_ChangeStance::Exit()
{
	dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

	m_EffectEvents.clear();
}

CPlayer_ChangeStance* CPlayer_ChangeStance::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_ChangeStance* pInstance = new CPlayer_ChangeStance();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_ChangeStance");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_ChangeStance::Free()
{
	__super::Free();
}