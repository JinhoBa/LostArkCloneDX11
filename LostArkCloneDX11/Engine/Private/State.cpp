#include "State.h"
#include "GameInstance.h"
#include "StateMachine.h"

CState::CState()
	:m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

void CState::Set_State()
{

}

HRESULT CState::Initilize()
{

	return S_OK;
}

void CState::Enter(void* pArg)
{

}

void CState::Update(_float fTimeDelta)
{

}

void CState::Exit()
{

}

void CState::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}