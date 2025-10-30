#include "pch.h"
#include "Attack_Normal_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Normal_Kamen::CAttack_Normal_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Normal_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CAttack_Normal_Kamen::Enter(void* pArg)
{
	m_eHitboxType = COLLIDER::OBB;
	m_iSkillID = 0;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	switch (*m_pPhase)
	{
	case PHASE::PHASE1:
		m_iEffectID = 1;
		m_pKamen->Set_Animation(16, false);
		break;

	case PHASE::PHASE2:
		m_iEffectID = 9;
		m_pKamen->Set_Animation(10, false);
		break;

	case PHASE::PHASE3:
		m_iSkillID = 2;
		m_iEffectID = 12;
		m_pKamen->Set_Animation(222, false);
		m_pKamen->Get_Transform()->TurnTo(m_pPlayerTransform->Get_Position());
		break;
	}

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);
	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOffset, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vExtends, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOrientation);

	Ready_EffectEvents();
}

void CAttack_Normal_Kamen::Update(_float fTimeDelta)
{
	Update_HitBox(fTimeDelta);

	Update_EffectTrack();

	if (m_pKamen->isAnimationFinish())
	{
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
	}

}

void CAttack_Normal_Kamen::Exit()
{
	m_EffectEvents.clear();
}

CAttack_Normal_Kamen* CAttack_Normal_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Normal_Kamen* pInstance = new CAttack_Normal_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Normal_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Normal_Kamen::Free()
{
	__super::Free();
}