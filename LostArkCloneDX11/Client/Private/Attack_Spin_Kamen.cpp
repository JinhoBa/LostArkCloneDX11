#include "pch.h"
#include "Attack_Spin_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Spin_Kamen::CAttack_Spin_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Spin_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CAttack_Spin_Kamen::Enter(void* pArg)
{
	m_eHitboxType = COLLIDER::OBB;
	m_iSkillID = 2;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	m_iEffectID = 3;

	
	m_pKamen->Set_Animation(29, false);

	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOffset, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vExtends, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOrientation);

	Ready_EffectEvents();
}

void CAttack_Spin_Kamen::Update(_float fTimeDelta)
{
	Update_HitBox(fTimeDelta);

	Update_EffectTrack();

	if (m_pKamen->isAnimationFinish())
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
}

void CAttack_Spin_Kamen::Exit()
{
	m_EffectEvents.clear();
}

CAttack_Spin_Kamen* CAttack_Spin_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Spin_Kamen* pInstance = new CAttack_Spin_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Spin_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Spin_Kamen::Free()
{
	__super::Free();
}