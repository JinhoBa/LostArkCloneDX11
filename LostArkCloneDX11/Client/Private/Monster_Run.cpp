#include "pch.h"
#include "Monster_Run.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Client_Struct.h"
#include "Monster.h"
#include "StateMachine.h"

CMonster_Run::CMonster_Run()
	:CMonster_State{}
{
}

HRESULT CMonster_Run::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Run::Enter(void* pArg)
{
	if (true == m_pMonster->isInBattle())
		m_pMonster->Set_Animation(ANIMATIONSLOT::RUN);
	else
		m_pMonster->Set_Animation(ANIMATIONSLOT::WALK);
}

void CMonster_Run::Update(_float fTimeDelta)
{
	if (Check_Hit())
		return;

	m_pMonster->Chase(fTimeDelta);

	if (true == m_pMonster->isInBattle())
	{
		if(m_pMonster->isInAttackRange())
			m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::TURN), nullptr);
	}
	else
	{
		/* 일정 시간 걷다가 Turn 으로 */
	}


}

void CMonster_Run::Exit()
{

}

CMonster_Run* CMonster_Run::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	CMonster_Run* pInstance = new CMonster_Run();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Run");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Run::Free()
{
	__super::Free();
}