#include "pch.h"
#include "Monster_Idle.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Idle::CMonster_Idle()
	:CMonster_State{}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_Idle::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Idle::Enter(void* pArg)
{
	if(true == m_pMonster->isInBattle())
		m_pMonster->Set_Animation(ANIMATIONSLOT::IDLE_BATTLE);
	else
		m_pMonster->Set_Animation(ANIMATIONSLOT::IDLE);
}

void CMonster_Idle::Update(_float fTimeDelta)
{
	if (Check_Hit())
		return;

	if (true == m_pMonster->isInBattle())
	{
		if (m_pMonster->isInAttackRange())
			m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::TURN), nullptr);
		else
			m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::RUN), nullptr);

	}
	else 	/* 전투 상태 아닐 때 */
	{
		/* Turn 으로 */
	}
	

}

void CMonster_Idle::Exit()
{

}

CMonster_Idle* CMonster_Idle::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	CMonster_Idle* pInstance = new CMonster_Idle();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Idle");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Idle::Free()
{
	__super::Free();
}