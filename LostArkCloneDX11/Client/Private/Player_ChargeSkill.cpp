#include "pch.h"
#include "Player_ChargeSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_ChargeSkill::CPlayer_ChargeSkill()
	:CPlayer_State{}
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
	CHARGESTATE_DESC* m_pSkill_Desc = static_cast<CHARGESTATE_DESC*>(pArg);

	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_pSkill_Desc->iSkillID);

	m_iKey = m_pSkill_Desc->iKey;
	
	m_eState = CHARGE_STATE::START;
	m_fChargeTime = 0.f;
	m_fMaxChargeTime = 1.5f;
	m_iSkillID = m_pSkill_Desc->iSkillID;

	if (12 == m_iSkillID)
	{
		m_iAnimStart = 126;
		m_iAnimLoop = 125;
		m_iAnimEnd = 124;
	}
	else
	{
		m_eState = CHARGE_STATE::LOOP;
		m_iAnimStart = 133;
		m_iAnimLoop = 134;
		m_iAnimEnd = 135;
	}

	m_pPlayer->Set_Animation(m_iAnimStart, false);
}

void CPlayer_ChargeSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	m_fChargeTime += fTimeDelta;
	// 피격 체크 추가
	m_pPlayer->Update_HitBox(m_iSkillID, 0);
	switch (m_eState)
	{
	case Client::CPlayer_ChargeSkill::START:
		if (m_pPlayer->isAnimationFinish())
		{
			m_eState = CPlayer_ChargeSkill::LOOP;
			m_pPlayer->Set_Animation(m_iAnimLoop, true, 0.f);
			m_fChargeTime = 0.f;
			m_pPlayer->Play_CameraAnimation(CAMERA_ANIM::ZOOMOUT);
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
			}
			else
			{
				m_eState = CPlayer_ChargeSkill::ATTACK;
				m_pPlayer->Set_Animation(m_iAnimLoop, false, 0.f);
			}
		}
		break;

	case Client::CPlayer_ChargeSkill::ATTACK:

		if (m_pPlayer->isAnimationFinish())
		{
			m_eState = CPlayer_ChargeSkill::END;
			m_pPlayer->Set_Animation(m_iAnimEnd, false, 0.f);
		}
		break;

	case Client::CPlayer_ChargeSkill::END:
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