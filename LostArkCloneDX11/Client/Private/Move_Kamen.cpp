#include "pch.h"
#include "Move_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CMove_Kamen::CMove_Kamen()
	:CState_Kamen{}
{
}

HRESULT CMove_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CMove_Kamen::Enter(void* pArg)
{
	m_fTimeAcc = 0.f;

	switch (*m_pPhase)
	{
	case PHASE::PHASE2:
		m_pKamen->Set_Animation(52, true);
		break;

	case PHASE::PHASE3:
		m_pKamen->Set_Animation(236, true);
		break;
	default:
		break;
	}

}

void CMove_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	_float fDistance = XMVector3Length(XMVectorSetY(m_pKamen->Get_Transform()->Get_Position(), 0.f) - XMVectorSetY(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_Position(), 0.f)).m128_f32[0];

	if (3.f <= m_fTimeAcc || false == m_pKamen->MoveToPlayer(fTimeDelta * 0.5f) || 3.f >= fDistance)
	{
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
	}

	m_pKamen->Turn(fTimeDelta);
}

void CMove_Kamen::Exit()
{

}

CMove_Kamen* CMove_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CMove_Kamen* pInstance = new CMove_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMove_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CMove_Kamen::Free()
{
	__super::Free();
}