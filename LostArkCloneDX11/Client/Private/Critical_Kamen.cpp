#include "pch.h"
#include "Critical_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"
#include "Attack_Kamen.h"
#include "Turn_Kamen.h"

CCritical_Kamen::CCritical_Kamen()
	:CState_Kamen{}
{
}

HRESULT CCritical_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCritical_Kamen::Enter(void* pArg)
{
	m_isLoop = false;
	m_fTimeAcc = 0.f;

	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		break;

	case PHASE::PHASE1:
		break;

	case PHASE::PHASE2:
		m_pKamen->Set_Animation(46, false);
		break;

	default:
		break;
	}


}

void CCritical_Kamen::Update(_float fTimeDelta)
{
	if (false == m_isLoop)
	{
		if (m_pKamen->isAnimationFinish())
		{
			m_isLoop = true;
			m_pKamen->Set_Animation(47, true);
		}
	}
	else
	{
		m_fTimeAcc += fTimeDelta;

		if (7.f <= m_fTimeAcc)
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
	}

	
}

void CCritical_Kamen::Exit()
{
}

CCritical_Kamen* CCritical_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CCritical_Kamen* pInstance = new CCritical_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CCritical_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CCritical_Kamen::Free()
{
	__super::Free();
}