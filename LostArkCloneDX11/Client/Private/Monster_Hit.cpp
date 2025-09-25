#include "pch.h"
#include "Monster_Hit.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Client_Struct.h"
#include "Monster.h"
#include "StateMachine.h"

CMonster_Hit::CMonster_Hit()
	:CMonster_State{}
{
}

HRESULT CMonster_Hit::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Hit::Enter(void* pArg)
{
	//MONSTER_HIT_DESC* pDesc = static_cast<MONSTER_HIT_DESC*>(pArg);

	m_eHitType = m_pMonster->Get_HitType();

	switch (m_eHitType)
	{
	case Client::HIT_TYPE::NORMAL:
		m_pMonster->Set_Animation(ANIMATIONSLOT::DMG);
		break;

	case Client::HIT_TYPE::PUSH:
		m_pMonster->Set_Animation(ANIMATIONSLOT::KNOKDOWN);
		break;

	case Client::HIT_TYPE::FLOAT:
		m_pMonster->Set_Animation(ANIMATIONSLOT::TWIST_KNOCKDOWN);
		break;

	}

	m_eState = STATE::HIT;
	m_fStiffnessTime = 0.f;
}

void CMonster_Hit::Update(_float fTimeDelta)
{
	m_fStiffnessTime += fTimeDelta;

	if(HIT_TYPE::NORMAL == m_eHitType)
	{
		if(m_fStiffnessTime >= 1.f)
		{
			m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::IDLE), nullptr);
			return;
		}
	}
	else
	{
		switch (m_eState)
		{
		case Client::CMonster_Hit::STATE::HIT:
			if (m_pMonster->isAnimationFinish())
			{
				switch (m_eHitType)
				{
				case Client::HIT_TYPE::NORMAL:

					return;
					break;

				case Client::HIT_TYPE::PUSH:
					m_pMonster->Set_Animation(ANIMATIONSLOT::KNOKDOWN_LAND);
					break;

				case Client::HIT_TYPE::FLOAT:
					m_pMonster->Set_Animation(ANIMATIONSLOT::TWIST_KNOCKDOWN_LAND);
					break;

				}
			}
			m_eState = STATE::LAND;

			break;

		case Client::CMonster_Hit::STATE::LAND:
			if (m_pMonster->isAnimationFinish())
			{
				if (m_pInfo->fHp <= 0.f)
					m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::DEAD), nullptr);
				else
				{
					m_pMonster->Set_Animation(ANIMATIONSLOT::STANDUP);
					m_eState = STATE::STANDUP;
				}

			}
			break;

		case Client::CMonster_Hit::STATE::STANDUP:
			if (m_pMonster->isAnimationFinish())
			{
				if (m_pInfo->fHp <= 0.f)
					m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::DEAD), nullptr);
				else
					m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::IDLE), nullptr);
			}
			break;
		}
	}

}

void CMonster_Hit::Exit()
{
	m_pMonster->Get_HitType();
}

CMonster_Hit* CMonster_Hit::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	CMonster_Hit* pInstance = new CMonster_Hit();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Hit");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Hit::Free()
{
	__super::Free();
}