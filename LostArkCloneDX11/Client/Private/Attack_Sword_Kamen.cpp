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

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	m_pKamen->Set_Animation(20, false);
	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDesc.vOffset, m_pSkillDesc->HitBoxDesc.vExtends);
}

void CAttack_Sword_Kamen::Update(_float fTimeDelta)
{
	Update_HitBox(fTimeDelta);

	if (m_pKamen->isAnimationFinish())
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
}

void CAttack_Sword_Kamen::Exit()
{
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