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

	m_pKamen->Set_Animation(196, true);

}

void CMove_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pKamen;

	if (false == m_bActive && 3.f <= m_fTimeAcc)
	{
	}

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