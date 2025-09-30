#include "pch.h"
#include "Intro_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CIntro_Kamen::CIntro_Kamen()
	:CState_Kamen{}
{
}

HRESULT CIntro_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CIntro_Kamen::Enter(void* pArg)
{
	m_fTimeAcc = 0.f;
	
	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		m_pKamen->Set_Animation(193, false);
		break;

	case PHASE::PHASE1:
		m_pKamen->Set_Animation(188, true);
		m_pKamen->Reposition();
		break;

	case PHASE::PHASE2:
		m_pKamen->Set_Animation(0, false);
		m_pKamen->Get_Transform()->Set_State(STATE::POSITION, XMVectorSet(35.f, 0.1f, 60.f, 1.f));
		break;

	case PHASE::PHASE3:
		m_pKamen->Set_Animation(239, true);
		break;

	default:
		break;
	}

}

void CIntro_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	
	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		
		break;

	case PHASE::PHASE1:
		break;

	case PHASE::PHASE2:
		if (m_pKamen->isAnimationFinish())
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		break;

	case PHASE::PHASE3:
		if(3.f <= m_fTimeAcc)
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		else
			m_pKamen->MoveToPlayer(fTimeDelta * 0.3f);
		break;

	default:
		break;
	}
}

void CIntro_Kamen::Exit()
{

}

CIntro_Kamen* CIntro_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CIntro_Kamen* pInstance = new CIntro_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIntro_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CIntro_Kamen::Free()
{
	__super::Free();
}