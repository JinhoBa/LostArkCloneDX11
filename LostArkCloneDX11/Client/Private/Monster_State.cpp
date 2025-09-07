#include "pch.h"
#include "Monster_State.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_State::CMonster_State()
	:CState{}, m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_State::Initilize(CStateMachine* pStateMachine, MONSTER* pType, CMonster* pMonster)
{
	if (nullptr == pStateMachine || nullptr == pType || nullptr == pMonster)
		return E_FAIL;

	m_pStateMachine = pStateMachine;
	Safe_AddRef(m_pStateMachine);

	m_pMonster = pMonster;
	Safe_AddRef(m_pMonster);

	m_pType = pType;

	return S_OK;
}

void CMonster_State::Enter(void* pArg)
{
}

void CMonster_State::Update(_float fTimeDelta)
{
}

void CMonster_State::Exit()
{
}

void CMonster_State::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
	Safe_Release(m_pStateMachine);
	Safe_Release(m_pMonster);
}