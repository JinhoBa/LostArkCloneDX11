#include "pch.h"
#include "Player_Dash.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Dash::CPlayer_Dash()
	:CPlayer_State{}
{
}

HRESULT CPlayer_Dash::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Dash::Enter(void* pArg)
{
	if(STANCE::FLURRY ==  *m_pPlayerStance)
		m_pPlayer->Set_Animation(127, false, 0.f);
	else
		m_pPlayer->Set_Animation(128, false, 0.f);

	m_bChangeStance = false;
}

void CPlayer_Dash::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	if (m_pGameInstance->Get_KeyDown(DIK_Z) && false == m_bChangeStance)
	{
		m_pPlayer->Change_Stance();
		m_bChangeStance = true;
	}

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_Dash::Exit()
{

}

CPlayer_Dash* CPlayer_Dash::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Dash* pInstance = new CPlayer_Dash();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Dash");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Dash::Free()
{
	__super::Free();
}