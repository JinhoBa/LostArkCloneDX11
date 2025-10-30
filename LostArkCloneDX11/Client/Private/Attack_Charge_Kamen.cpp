#include "pch.h"
#include "Attack_Charge_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Charge_Kamen::CAttack_Charge_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Charge_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CAttack_Charge_Kamen::Enter(void* pArg)
{
	ATTACK_KAMEN_DESC* pDesc = static_cast<ATTACK_KAMEN_DESC*>(pArg);


	m_iSkillID = 1;
	m_eHitboxType = COLLIDER::OBB;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	m_eState = STATE::START;
	if (PHASE::PHASE1 == (*m_pPhase))
	{
		m_iEffectID = 2;
		m_pKamen->Set_Animation(36, false);
	}
	else
	{
		m_iEffectID = 13;
		m_pKamen->Set_Animation(44, false);
	}
	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOffset, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vExtends, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOrientation);

	Ready_EffectEvents();
}

void CAttack_Charge_Kamen::Update(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CAttack_Charge_Kamen::STATE::START:
		if (m_pKamen->isAnimationFinish())
		{
			m_eState = STATE::LOOP;
			if (PHASE::PHASE1 == (*m_pPhase))
			{
				m_pKamen->Set_Animation(37, false);
				m_pGameManager->Add_Effect(EFFECT::PARTICLE, 11, &(m_pKamen->Get_Transform()->Get_WorldMatrix()), CHARACTER::BOSS);
				m_pGameInstance->Play_Sound(L"Kamen1_Skill_4_0.wav", CHANNELID::EFFECT, 0.7f);
			}
			else
			{
				m_pGameManager->Add_Effect(EFFECT::MESH, 33, &m_pKamen->Get_Transform()->Get_WorldMatrix(), CHARACTER::BOSS);
				m_pKamen->Set_Animation(45, false);
			}
		}
		break;

	case Client::CAttack_Charge_Kamen::STATE::LOOP:
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= 1.f)
		{
			m_eState = STATE::END;
			if (PHASE::PHASE1 == (*m_pPhase))
				m_pKamen->Set_Animation(38, false);
			else
				m_pKamen->Set_Animation(46, false);
		}
		break;

	case Client::CAttack_Charge_Kamen::STATE::END:

		if (PHASE::PHASE1 == (*m_pPhase))
		{
	
		}
		Update_HitBox(fTimeDelta);
		Update_EffectTrack();

		if (m_pKamen->isAnimationFinish())
		{
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		}
		break;

	default:
		break;
	}
}

void CAttack_Charge_Kamen::Exit()
{
	m_EffectEvents.clear();
}

CAttack_Charge_Kamen* CAttack_Charge_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Charge_Kamen* pInstance = new CAttack_Charge_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Charge_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Charge_Kamen::Free()
{
	__super::Free();
}