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
		m_iEffectID = 0;
		break;

	case PHASE::PHASE1:
		m_iEffectID = 8;
		m_pKamen->Reposition();
		m_pKamen->Set_Animation(188, true);
		break;

	case PHASE::PHASE2:
		m_iEffectID = 8;
		m_pKamen->Set_Animation(0, false);
		m_pKamen->Get_Transform()->Set_State(STATE::POSITION, XMVectorSet(35.f, 0.1f, 60.f, 1.f));
		break;

	case PHASE::PHASE3:
		m_iEffectID = 0;
		m_pKamen->Set_Animation(239, true);
		break;

	default:
		break;
	}


	Ready_EffectEvents();
}

void CIntro_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	
	switch (*m_pPhase)
	{
	case PHASE::INTRO:
		Update_EffectTrack();
		break;

	case PHASE::PHASE1:
		break;

	case PHASE::PHASE2:
		Update_EffectTrack();
		if (m_pKamen->isAnimationFinish())
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		break;

	case PHASE::PHASE3:
		Update_EffectTrack();
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
	m_EffectEvents.clear();
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