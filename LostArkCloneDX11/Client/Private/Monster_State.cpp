#include "pch.h"
#include "Monster_State.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"
#include "Monster_Hit.h"

CMonster_State::CMonster_State()
	:CState{}, m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_State::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (nullptr == pStateMachine || nullptr == pInfo || nullptr == pMonster)
		return E_FAIL;

	m_pStateMachine = pStateMachine;
	Safe_AddRef(m_pStateMachine);

	m_pMonster = pMonster;
	Safe_AddRef(m_pMonster);

	m_pInfo = pInfo;

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

_bool CMonster_State::Check_Hit()
{

	return false;
}

void CMonster_State::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
	Safe_Release(m_pStateMachine);
	Safe_Release(m_pMonster);
}