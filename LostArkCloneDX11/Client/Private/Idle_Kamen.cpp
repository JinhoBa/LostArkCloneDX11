#include "pch.h"
#include "Idle_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"
#include "Attack_Kamen.h"
#include "Turn_Kamen.h"

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
	++m_iSkillID;

	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		m_pKamen->Set_Animation(193, false);
		break;

	case PHASE::PHASE1:
		m_pKamen->Set_Animation(188, true);
		m_pKamen->Reposition();
		if (5 < m_iSkillID)
			m_iSkillID = 1;
		break;

	case PHASE::PHASE2:
		m_pKamen->Set_Animation(50, true);
		m_iSkillID = 1;
		break;

	case PHASE::PHASE3:
		m_pKamen->Set_Animation(235, true);
		break;

	default:
		break;
	}

}

void CIdle_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	CTurn_Kamen::TURN_KAMEN_DESC TurnDesc = {};

	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		m_pKamen->Set_Animation(193, false);
		break;

	case PHASE::PHASE1:
	case PHASE::PHASE2:
		if (true == m_pKamen->Turn(fTimeDelta))
		{
			switch (m_iSkillID)
			{
			case 1:
				TurnDesc.iKamenStateID = static_cast<_uint>(CKamen::KAMENSTATE::ATTACK_NORMAL);
				break;

			case 2:
				TurnDesc.iKamenStateID = static_cast<_uint>(CKamen::KAMENSTATE::ATTACK_CHARGE);
				break;

			case 3:
				TurnDesc.iKamenStateID = static_cast<_uint>(CKamen::KAMENSTATE::ATTACK_SPIN);
				break;

			case 4:
				TurnDesc.iKamenStateID = static_cast<_uint>(CKamen::KAMENSTATE::ATTACK_SWORD);
				break;

			case 5:
				TurnDesc.iKamenStateID = static_cast<_uint>(CKamen::KAMENSTATE::ATTACK_COMBO);
				break;

			default:
				break;
			}

			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::TRUN), &TurnDesc);
			return;
		}
		else
		{
			if (1.f <= m_fTimeAcc)
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
		}
		break;

	case PHASE::PHASE3:
		if(1.f < m_fTimeAcc)
		{
			TurnDesc.iKamenStateID = 1;
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::TRUN), &TurnDesc);
		}
		break;

	default:
		break;
	}

	

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