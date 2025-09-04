#include "pch.h"
#include "Player_Hit.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Hit::CPlayer_Hit()
	:CPlayer_State{}
{
}

HRESULT CPlayer_Hit::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Hit::Enter(void* pArg)
{
	// 피격 종류 받아와서 처리
	// 피격마다 애니메이션 횟수가 다를 예정

	/* 기본 피격 */
	if (STANCE::FLURRY == *m_pPlayerStance)
		m_pPlayer->Set_Animation(23, false);
	else
		m_pPlayer->Set_Animation(24, false);

	m_pPlayer->Change_Stance();
}

void CPlayer_Hit::Update(_float fTimeDelta)
{
	// 기상기 체크


	// 기본 회복
	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_Hit::Exit()
{

}

CPlayer_Hit* CPlayer_Hit::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Hit* pInstance = new CPlayer_Hit();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Hit");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Hit::Free()
{
	__super::Free();
}