#include "pch.h"
#include "Monster_Attack.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Attack::CMonster_Attack()
	:CMonster_State{}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_Attack::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster, _uint iNumAttackAnimation)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	m_iNumAttackAnimation = iNumAttackAnimation;

	m_iAttackIndex = 0;

	return S_OK;
}

void CMonster_Attack::Enter(void* pArg)
{
	ANIMATIONSLOT eType = static_cast<ANIMATIONSLOT>(ENUM_TO_INT(ANIMATIONSLOT::ATTACK_1) + m_iAttackIndex);
	m_pMonster->Set_Animation(eType);

	m_bStartHit = {false};
	m_isActive = {false};
	m_iNumHit = 0;
	m_fTimeAcc = 0.f;

	m_pSkill_Info = m_pGameManager->Get_Monster_SkillInfo_Prt(m_pInfo->iMonsterID, m_iAttackIndex);
}

void CMonster_Attack::Update(_float fTimeDelta)
{
	if (false == m_bStartHit)
	{
		if (m_pMonster->Get_TrackPositon() >= m_pSkill_Info->HitBoxDesc.fStartTime)
		{
			m_bStartHit = true;
			m_isActive = true;
			m_pMonster->Set_HitBox(m_pSkill_Info->HitBoxDesc.vOffset,m_pSkill_Info->HitBoxDesc.vExtends);
		}
	}
	else
	{
		if (m_iNumHit < m_pSkill_Info->iNumAttack)
		{
			m_fTimeAcc += fTimeDelta;

			if (true == m_isActive)
			{
				m_pMonster->Update_HitBox(m_iAttackIndex, m_iNumHit);
				if (m_fTimeAcc >= m_pSkill_Info->HitBoxDesc.fDuration)
				{
					m_isActive = false;
					++m_iNumHit;
					m_fTimeAcc = 0.f;
				}
			}
			else
			{
				if (m_fTimeAcc >= m_pSkill_Info->HitBoxDesc.fInterval)
				{
					m_isActive = true;
					m_fTimeAcc = 0.f;
					
				}
			}
		}
	}

	if (m_pMonster->isAnimationFinish())
	{
		m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::IDLE), nullptr);
	}
}

void CMonster_Attack::Exit()
{
	++m_iAttackIndex;

	if (m_iAttackIndex >= m_iNumAttackAnimation)
		m_iAttackIndex = 0;
}


CMonster_Attack* CMonster_Attack::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster, _uint iNumAttackAnimation)
{
	CMonster_Attack* pInstance = new CMonster_Attack();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster, iNumAttackAnimation)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Attack");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Attack::Free()
{
	__super::Free();
}