#include "pch.h"
#include "Skill_Player.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"

CSkill_Player::CSkill_Player()
	:CState_Player{}
{
}

HRESULT CSkill_Player::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Player::Enter(void* pArg)
{
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;
}

void CSkill_Player::Update(_float fTimeDelta)
{
}

void CSkill_Player::Exit()
{
}

void CSkill_Player::Update_Hitbox(_float fTimeDelta)
{
	if (m_isStartHit == false && m_pPlayer->Get_TrackPositon() >= m_pSkillInfo->HitBoxDesc.fStartTime)
	{
		m_isStartHit = m_isActiveHitBox = true;
	}

	if (true == m_isStartHit && m_iAttackCount < m_pSkillInfo->iNumAttack)
	{
		if (m_isActiveHitBox)
		{
			m_fTimeAcc += fTimeDelta;
			m_pPlayer->Update_HitBox(m_iSkillID, m_iAttackCount);

			if (m_pSkillInfo->HitBoxDesc.fDuration <= m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				m_isActiveHitBox = false;
			}
		}
		else
		{
			m_fTimeAcc += fTimeDelta;
			if (m_pSkillInfo->HitBoxDesc.fInterval <= m_fTimeAcc)
			{
				m_isActiveHitBox = true;
				m_fTimeAcc = 0.f;
				++m_iAttackCount;
			}
		}
	}
}

void CSkill_Player::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}