#include "pch.h"
#include "Attack_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Kamen::CAttack_Kamen()
	:CState_Kamen{}
{
}

HRESULT CAttack_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CAttack_Kamen::Enter(void* pArg)
{
}

void CAttack_Kamen::Update(_float fTimeDelta)
{

}

void CAttack_Kamen::Exit()
{

}

void CAttack_Kamen::Free()
{
	__super::Free();
}