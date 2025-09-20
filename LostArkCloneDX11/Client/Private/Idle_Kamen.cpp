#include "pch.h"
#include "Idle_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"
#include "Attack_Kamen.h"

CIdle_Kamen::CIdle_Kamen()
	:CState_Kamen{}
{
}

HRESULT CIdle_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	m_iSkillID = 0;

	return S_OK;
}

void CIdle_Kamen::Enter(void* pArg)
{
	m_fTimeAcc = 0.f;

	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		m_pKamen->Set_Animation(193, false);
		break;

	case PHASE::PHASE1:
		m_pKamen->Set_Animation(188, true);
		break;

	default:
		break;
	}

	++m_iSkillID;

	if (5 < m_iSkillID)
		m_iSkillID = 1;

}

void CIdle_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (2.f <= m_fTimeAcc)
	{
		switch (m_iSkillID)
		{
		case 1:
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
			break;
		case 2:
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_CHARGE), nullptr);
			break;
		case 3:
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_COMBO), nullptr);
			break;
		case 4:
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_SWORD), nullptr);
			break;
		case 5:
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_SPIN), nullptr);
			break;

		default:
			break;
		}
	}

	/*switch (*m_pPhase)
	{
	case PHASE::PHASE1:

		break;

	default:
		break;
	}*/

}

void CIdle_Kamen::Exit()
{
}

CIdle_Kamen* CIdle_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CIdle_Kamen* pInstance = new CIdle_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIdel_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CIdle_Kamen::Free()
{
	__super::Free();
}