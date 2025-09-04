#include "pch.h"
#include "Player_ChangeStance.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_ChangeStance::CPlayer_ChangeStance()
	:CPlayer_State{}
{
}

HRESULT CPlayer_ChangeStance::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_ChangeStance::Enter(void* pArg)
{
	if (STANCE::FLURRY == *m_pPlayerStance)
		m_pPlayer->Set_Animation(40, false);
	else
		m_pPlayer->Set_Animation(41, false);

	m_pPlayer->Change_Stance();
}

void CPlayer_ChangeStance::Update(_float fTimeDelta)
{
	if (m_pPlayer->isAnimationFinish())
	{
		if(m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_ChangeStance::Exit()
{

}

CPlayer_ChangeStance* CPlayer_ChangeStance::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_ChangeStance* pInstance = new CPlayer_ChangeStance();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_ChangeStance");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_ChangeStance::Free()
{
	__super::Free();
}