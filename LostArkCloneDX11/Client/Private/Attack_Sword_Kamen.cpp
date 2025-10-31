#include "pch.h"
#include "Attack_Sword_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Sword_Kamen::CAttack_Sword_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Sword_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CAttack_Sword_Kamen::Enter(void* pArg)
{
	m_iSkillID = 4;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	switch (*m_pPhase)
	{
	case PHASE::PHASE1:
		m_eHitboxType = COLLIDER::SPHERE;
		m_iEffectID = 6;
		m_pKamen->Set_Animation(20, false);
		break;

	case PHASE::PHASE2:
		m_iEffectID = 10;
		m_eHitboxType = COLLIDER::OBB;
		m_pKamen->Set_Animation(16, false);
		break;

	case PHASE::PHASE3:
		m_iSkillID = 0;
		m_iEffectID = 14;
		m_eHitboxType = COLLIDER::OBB;
		m_pKamen->Set_Animation(187, false);
		break;

	default:
		break;
	}

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	Ready_EffectEvents();
}

void CAttack_Sword_Kamen::Update(_float fTimeDelta)
{
	Update_HitBox(fTimeDelta);

	Update_EffectTrack();

	if (m_pKamen->isAnimationFinish())
	{
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
	}
}

void CAttack_Sword_Kamen::Exit()
{
	m_EffectEvents.clear();
}

CAttack_Sword_Kamen* CAttack_Sword_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Sword_Kamen* pInstance = new CAttack_Sword_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Sword_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Sword_Kamen::Free()
{
	__super::Free();
}