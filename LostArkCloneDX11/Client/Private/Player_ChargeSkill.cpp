#include "pch.h"
#include "Player_ChargeSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"

CPlayer_ChargeSkill::CPlayer_ChargeSkill()
	:CSkill_Player{}
{
}

HRESULT CPlayer_ChargeSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_ChargeSkill::Enter(void* pArg)
{
	__super::Enter(pArg);

	CHARGESTATE_DESC* m_pSkill_Desc = static_cast<CHARGESTATE_DESC*>(pArg);

	m_iSkillID = m_pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_iSkillID);

	m_iKey = m_pSkill_Desc->iKey;
	
	m_eState = CHARGE_STATE::START;
	m_fChargeTime = 0.f;
	m_fMaxChargeTime = 1.5f;
	
	m_isSpawEffect = true;

	if (12 == m_iSkillID) // 적령포
	{
		m_iAnimStart = 126;
		m_iAnimLoop = 125;
		m_iAnimEnd = 124;
		m_pGameInstance->Play_Sound(L"RedDragonFlight_Start.wav", CHANNELID::SKILL_PLAYER, 0.2f);
	}
	else
	{
		m_eState = CHARGE_STATE::LOOP;
		m_iAnimStart = 133;
		m_iAnimLoop = 134;
		m_iAnimEnd = 135;
		m_pGameInstance->Play_Sound(L"ScorchingWaveCharge1.wav", CHANNELID::SKILL_PLAYER, 0.2f);
	}

	m_pPlayer->Set_Animation(m_iAnimStart, false);
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends);

	Ready_EffectTrack();

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();
}

void CPlayer_ChargeSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	m_fChargeTime += fTimeDelta;
	// 피격 체크 추가

	switch (m_eState)
	{
	case Client::CPlayer_ChargeSkill::START:
		if (m_pPlayer->isAnimationFinish())
		{
			m_eState = CPlayer_ChargeSkill::LOOP;
			m_pPlayer->Set_Animation(m_iAnimLoop, true, 0.f);
			m_fChargeTime = 0.f;
			m_pGameInstance->Bind_Camera(TEXT("Camera_ChargeSkill"));

			m_pGameInstance->Play_Sound(L"RedDragonFlight_Charge.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		}
		break;

	case Client::CPlayer_ChargeSkill::LOOP:
		m_pPlayer->TurnToCursor();

		if(12 == m_iSkillID)
			m_pPlayer->Set_ChargeSkill_Desc(true, m_fChargeTime);

		if (false == m_pGameInstance->Get_KeyPressing(m_iKey) || m_fMaxChargeTime <= m_fChargeTime)
		{
			if(12 == m_iSkillID)
			{
				m_eState = CPlayer_ChargeSkill::END;
				m_pPlayer->Set_Animation(m_iAnimEnd, false);

				m_pGameInstance->StopSound(CHANNELID::SKILL_PLAYER);
				m_pGameInstance->Play_Sound(L"RedDragonFlight_Shoot.wav", CHANNELID::SKILL_PLAYER, 0.2f);
			}
			else
			{
				m_eState = CPlayer_ChargeSkill::ATTACK;
				m_pPlayer->Set_Animation(m_iAnimLoop, false, 0.f);

				m_pGameInstance->Play_Sound(L"ScorchingWave_Shoot.wav", CHANNELID::SKILL_PLAYER, 0.4f);
			}
		}
		break;

	case Client::CPlayer_ChargeSkill::ATTACK:
		if(13 == m_iSkillID)
		{
			Update_Hitbox(fTimeDelta);
			Update_EffectTrack();
		}

		if (m_pPlayer->isAnimationFinish())
		{


			m_eState = CPlayer_ChargeSkill::END;
			m_pPlayer->Set_Animation(m_iAnimEnd, false, 0.f);
		}
		break;

	case Client::CPlayer_ChargeSkill::END:
		if (12 == m_iSkillID)
		{
			Update_Hitbox(fTimeDelta);
			Update_EffectTrack();
		}
		if (m_pPlayer->isAnimationFinish())
		{
			m_pPlayer->Set_ChargeSkill_Desc(false, 0.f);
			if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
			else
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		}
		break;

	default:
		break;
	}

}

void CPlayer_ChargeSkill::Exit()
{
	m_pGameInstance->StopSound(CHANNELID::SKILL_PLAYER);

	if (12 == m_iSkillID)
		m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"), true);

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

	m_EffectEvents.clear();
}



CPlayer_ChargeSkill* CPlayer_ChargeSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_ChargeSkill* pInstance = new CPlayer_ChargeSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_ChargeSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_ChargeSkill::Free()
{
	__super::Free();
}