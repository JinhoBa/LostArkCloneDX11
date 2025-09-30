#include "pch.h"
#include "State_Player.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"

CState_Player::CState_Player()
	:CState{}, m_pGameManager{CGameManager::GetInstance()}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CState_Player::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
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

void CState_Player::Enter(void* pArg)
{
}

void CState_Player::Update(_float fTimeDelta)
{
}

void CState_Player::Exit()
{
}

_bool CState_Player::Check_Dash()
{
	if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
	{
		return m_pGameManager->Use_Skill(16);
	}

	return false;
}

void CState_Player::Toggle_HUD()
{
	for (auto& pObject : m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Canvars")))
		pObject->Toggle_Visible();
	for (auto& pObject : m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_HUD")))
		pObject->Toggle_Visible();
}

void CState_Player::Free()
{
	__super::Free();

	//Safe_Release(m_pPlayer);
	//Safe_Release(m_pStateMachine);
	Safe_Release(m_pGameManager);
}