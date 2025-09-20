#include "pch.h"
#include "State_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Kamen.h"
#include "StateMachine.h"

CState_Kamen::CState_Kamen()
	:CState{}, m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CState_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (nullptr == pDesc)
		return E_FAIL;

	m_pStateMachine = pDesc->pStateMachine;
	m_pKamen = pDesc->pKamen;
	m_pPhase = pDesc->pPhase;
	
	return S_OK;
}

void CState_Kamen::Enter(void* pArg)
{
}

void CState_Kamen::Update(_float fTimeDelta)
{
}

void CState_Kamen::Exit()
{
}

void CState_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}