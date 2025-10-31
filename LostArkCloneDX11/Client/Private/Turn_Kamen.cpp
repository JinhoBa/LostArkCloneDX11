#include "pch.h"
#include "Turn_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CTurn_Kamen::CTurn_Kamen()
	:CState_Kamen{}
{
}

HRESULT CTurn_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	m_iAttackCount = 0;

	return S_OK;
}

void CTurn_Kamen::Enter(void* pArg)
{
	TURN_KAMEN_DESC* pDesc = static_cast<TURN_KAMEN_DESC*>(pArg);

	m_iKamenStateID = pDesc->iKamenStateID;


	m_isRight = false;
	m_fDistance = 0.f;

	_float fKamenX = XMVectorGetX(m_pKamen->Get_Transform()->Get_Position());
	_float fPlayerX = XMVectorGetX(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_Position());

	m_fDistance = XMVector3Length(XMVectorSetY(m_pKamen->Get_Transform()->Get_Position(), 0.f) - XMVectorSetY(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_Position(), 0.f)).m128_f32[0];

	if (fKamenX < fPlayerX)
		m_isRight = true;

	switch (*m_pPhase)
	{
	case PHASE::PHASE1:
		if(m_isRight)
			m_pKamen->Set_Animation(200, false);
		else
			m_pKamen->Set_Animation(198, false);
		break;

	case PHASE::PHASE2:
		if (m_isRight)
			m_pKamen->Set_Animation(238, false);
		else
			m_pKamen->Set_Animation(237, false);
		break;

	case PHASE::PHASE3:
		if (m_isRight)
			m_pKamen->Set_Animation(54, false);
		else
			m_pKamen->Set_Animation(53, false);
		break;
	}

}

void CTurn_Kamen::Update(_float fTimeDelta)
{
	if(!m_pKamen->Turn(fTimeDelta))
	{
		switch (*m_pPhase)
		{
		case PHASE::PHASE1:
			m_pStateMachine->Change_State(m_pKamen->Get_State(static_cast<CKamen::KAMENSTATE>(m_iKamenStateID)), nullptr);
			break;

		case PHASE::PHASE2:
			if(2 < m_iAttackCount  && m_pKamen->Reposition())
			{
				m_iAttackCount = 0;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::CLASH), nullptr);
			}
			else if(3.f > m_fDistance)
			{
				++m_iAttackCount;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
			}
			else
			{
				++m_iAttackCount;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_SWORD), nullptr);
			}
			break;

		case PHASE::PHASE3:
			if(3 > m_iAttackCount)
			{
				++m_iAttackCount;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
			}
			else if(3 == m_iAttackCount)
			{
				++m_iAttackCount;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_SWORD), nullptr); // ±ÙÁ¢
			}
			else if((4 == m_iAttackCount))
			{
				++m_iAttackCount;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_CHARGE), nullptr);
			}
			else
			{
				m_iAttackCount = 0;
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::MOVE), nullptr);
			}
			
			break;
		}

	
	}

}

void CTurn_Kamen::Exit()
{

}

CTurn_Kamen* CTurn_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CTurn_Kamen* pInstance = new CTurn_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CTurn_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CTurn_Kamen::Free()
{
	__super::Free();
}