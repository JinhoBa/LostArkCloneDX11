#include "pch.h"
#include "Player_NormalSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Effect_Ground.h"
#include "WeaponEffect_Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"

CPlayer_NormalSkill::CPlayer_NormalSkill()
	:CSkill_Player{}
{
}

HRESULT CPlayer_NormalSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_NormalSkill::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pPlayer->TurnToCursor();

	NORMALSKILL_DESC* m_pSkill_Desc = static_cast<NORMALSKILL_DESC*>(pArg);

	m_iSkillID = m_pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_pSkill_Desc->iSkillID);

	if(10 == m_iSkillID)
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop, 0.f);
	else
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop);

	/* Sound */
	Play_SkillSound();

	/* HitBox */
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends);

	/* Effects */
	Ready_EffectTrack();

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

}

void CPlayer_NormalSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	Update_Hitbox(fTimeDelta);
	
	Update_EffectTrack();

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_NormalSkill::Exit()
{
	m_pGameInstance->StopSound(CHANNELID::SKILL_PLAYER);

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

	Clear_Events();

	m_pPlayer->Set_SkillID(99);

	m_EffectEvents.clear();

}

void CPlayer_NormalSkill::Play_SkillSound()
{
	switch (m_iSkillID)
	{
	case 1:
		m_pGameInstance->Play_Sound(L"Moonblade_Fury.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 3:
		m_pGameInstance->Play_Sound(L"RingofLight.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 4:
		m_pGameInstance->Play_Sound(L"DescentOfMoon.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 5:
		m_pGameInstance->Play_Sound(L"EclipsePath.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 6:
		m_pGameInstance->Play_Sound(L"SkyWheel.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 7:
		m_pGameInstance->Play_Sound(L"CrescentMoon.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 10:
		m_pGameInstance->Play_Sound(L"SolarEclispe.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 11:
		m_pGameInstance->Play_Sound(L"LightBlades.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	
	default:
		break;
	}
}



CPlayer_NormalSkill* CPlayer_NormalSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_NormalSkill* pInstance = new CPlayer_NormalSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_NormalSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_NormalSkill::Free()
{
	__super::Free();
}