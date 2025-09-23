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
			m_pKamen->Set_Animation(54, false);
		else
			m_pKamen->Set_Animation(53, false);

		break;

	default:
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
			if(m_fDistance < 5.f)
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
			else
				m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
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