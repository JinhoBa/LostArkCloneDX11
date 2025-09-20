#include "pch.h"
#include "Player_State.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"

CPlayer_State::CPlayer_State()
	:CState{}, m_pGameManager{CGameManager::GetInstance()}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CPlayer_State::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (nullptr == pStateMachine || nullptr == pStance || nullptr == pPlayer)
		return E_FAIL;

	m_pPlayerStance = pStance;
	m_pStateMachine = pStateMachine;
	m_pPlayer = pPlayer;

	//Safe_AddRef(m_pStateMachine);
	//Safe_AddRef(m_pPlayer);

	return S_OK;
}

void CPlayer_State::Enter(void* pArg)
{
}

void CPlayer_State::Update(_float fTimeDelta)
{
}

void CPlayer_State::Exit()
{
}

_bool CPlayer_State::Check_Dash()
{
	if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
	{
		return m_pGameManager->Use_Skill(16);
	}

	return false;
}

void CPlayer_State::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
	//Safe_Release(m_pStateMachine);
	//Safe_Release(m_pPlayer);
}