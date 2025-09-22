#include "pch.h"
#include "Player_Hit.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Hit::CPlayer_Hit()
	:CState_Player{}
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
	PLAYER_HIT_DESC* pDesc = static_cast<PLAYER_HIT_DESC*>(pArg);

	m_HitType = pDesc->eType;

	switch (m_HitType)
	{
	case Client::HIT_TYPE::NORMAL:
		/* 기본 피격 */
		if (STANCE::FLURRY == *m_pPlayerStance)
			m_pPlayer->Set_Animation(23, false, 0.3f);
		else
			m_pPlayer->Set_Animation(24, false, 0.3f);
		break;

	case Client::HIT_TYPE::PUSH:
		m_pPlayer->Get_Transform()->TurnTo(XMVectorSetW(XMLoadFloat3(&pDesc->vPosition), 1.f));
		m_pPlayer->Set_Animation(25, false);
		break;
	case Client::HIT_TYPE::FLOAT:
		m_pPlayer->Get_Transform()->TurnTo(XMVectorSetW(XMLoadFloat3(&pDesc->vPosition), 1.f));
		m_pPlayer->Set_Animation(18, false);
		break;

	default:
		break;
	}

	m_iNumAnimation = 0;
	m_fDownTime = 0.f;
	m_eState = STATE::HIT;

}

void CPlayer_Hit::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	switch (m_HitType)
	{
	case Client::HIT_TYPE::NORMAL:
		// 기본 회복
		if (m_pPlayer->isAnimationFinish())
		{
			if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
			else
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		}
		break;

	case Client::HIT_TYPE::PUSH:
		// 기상기 체크
		if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
		{
			if (m_pGameManager->Use_Skill(17))
			{
				m_pPlayer->Set_Animation(217, false);
				m_eState = STATE::RECOVER;
			}
		}
		
		switch (m_eState)
		{
		case Client::CPlayer_Hit::STATE::HIT:
			if (m_pPlayer->isAnimationFinish())
			{
				m_eState = STATE::DOWN;
			}
				
			break;
		case Client::CPlayer_Hit::STATE::DOWN:
			m_fDownTime += fTimeDelta;

			if (1.f <= m_fDownTime)
			{
				m_eState = STATE::RECOVER;
				m_pPlayer->Set_Animation(44, false);
			}
			break;
		case Client::CPlayer_Hit::STATE::RECOVER:
			m_pPlayer->Set_SkillID(17);
			if (m_pPlayer->isAnimationFinish())
			{
				if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
					m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
				else
					m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
			}

			break;
		}
		

		break;
	case Client::HIT_TYPE::FLOAT:
		// 기상기 체크
		if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
		{
			if (m_pGameManager->Use_Skill(17))
			{
				m_pPlayer->Set_Animation(217, false);
				m_eState = STATE::RECOVER;
			}
		}

		switch (m_eState)
		{
		case Client::CPlayer_Hit::STATE::HIT:
			if (m_pPlayer->isAnimationFinish())
			{
				m_eState = STATE::DOWN;
			}

			break;
		case Client::CPlayer_Hit::STATE::DOWN:
			m_fDownTime += fTimeDelta;

			if (1.f <= m_fDownTime)
			{
				m_eState = STATE::RECOVER;
				m_pPlayer->Set_Animation(44, false);
			}
			break;
		case Client::CPlayer_Hit::STATE::RECOVER:
			m_pPlayer->Set_SkillID(17);
			if (m_pPlayer->isAnimationFinish())
			{
				if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
					m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
				else
					m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
			}

			break;
		}

	default:
		break;
	}
}

void CPlayer_Hit::Exit()
{
	m_pPlayer->Set_SkillID(99);
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