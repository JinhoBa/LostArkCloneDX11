#include "pch.h"
#include "Dead_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CDead_Kamen::CDead_Kamen()
	:CState_Kamen{}
{
}

HRESULT CDead_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CDead_Kamen::Enter(void* pArg)
{
	switch (*m_pPhase)
	{
	case PHASE::PHASE3:
		m_pKamen->Set_Animation(228, false);
		break;
	default:
		break;
	}

}

void CDead_Kamen::Update(_float fTimeDelta)
{
	
}

void CDead_Kamen::Exit()
{

}

CDead_Kamen* CDead_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CDead_Kamen* pInstance = new CDead_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CDead_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CDead_Kamen::Free()
{
	__super::Free();
}