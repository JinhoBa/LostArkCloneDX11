#include "pch.h"
#include "Player_Clash.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Clash::CPlayer_Clash()
	:CState_Player{}
{
}

HRESULT CPlayer_Clash::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Clash::Enter(void* pArg)
{
	m_isFinish = false;
	m_fTimeAcc = 0.f;

	m_eState = STATE::WAIT;

	m_pPlayer->Get_Transform()->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, 0.1f, 48.f, 1.f));
}

void CPlayer_Clash::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	switch (m_eState)
	{
	case Client::CPlayer_Clash::STATE::WAIT:
		if (5.f <= m_fTimeAcc)
		{
			m_pPlayer->Set_Animation(7, false);
			m_fTimeAcc = 0.f;
			m_eState = STATE::START;
		}
		
		break;

	case Client::CPlayer_Clash::STATE::START:
		if (m_pPlayer->isAnimationFinish())
		{
			m_pPlayer->Set_Animation(6, true);
			m_eState = STATE::LOOP;
		}
		break;
	case Client::CPlayer_Clash::STATE::LOOP:
		if(m_pGameManager->isFinshed_Clash())
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		break;

	default:
		break;
	}


}

void CPlayer_Clash::Exit()
{

}

CPlayer_Clash* CPlayer_Clash::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Clash* pInstance = new CPlayer_Clash();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Clash");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Clash::Free()
{
	__super::Free();
}