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
		m_iSkillID = 19;
		m_pPlayer->Set_Animation(40, false);
		m_pGameInstance->Play_Sound(L"ToFlurry3.wav", CHANNELID::SKILL_PLAYER, 0.5f);
	}
	else
	{
		m_iSkillID = 20;
		m_pPlayer->Set_Animation(41, false);
		m_pGameInstance->Play_Sound(L"ToFocus3.wav", CHANNELID::SKILL_PLAYER, 0.5f);
	}

	m_pPlayer->Change_Stance();

	Ready_EffectTrack();

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

	Update_EffectTrack();
}

void CPlayer_ChangeStance::Exit()
{
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