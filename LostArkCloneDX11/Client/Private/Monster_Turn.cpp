#include "pch.h"
#include "Monster_Turn.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Turn::CMonster_Turn()
	:CMonster_State{}
{
}

HRESULT CMonster_Turn::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Turn::Enter(void* pArg)
{
	/*if (true == m_pMonster->isInBattle())
	{

	}
	else
	{

	}*/

	//if(MONSTER::NORMAL == *m_pType)

	/* ¿ÞÂÊ */
	//m_pMonster->Set_Animation(ANIMATIONSLOT::TURN_L);

	/* ¿À¸¥ÂÊ */
	//m_pMonster->Set_Animation(14, true);

	if (0.f >= m_pInfo->fHp)
		m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::DEAD), nullptr);

}

void CMonster_Turn::Update(_float fTimeDelta)
{
	if (Check_Hit())
		return;

	if (false == m_pMonster->Turn(fTimeDelta))
	{
		m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::ATTACK), nullptr);
	}
}

void CMonster_Turn::Exit()
{

}

CMonster_Turn* CMonster_Turn::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	CMonster_Turn* pInstance = new CMonster_Turn();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Turn");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Turn::Free()
{
	__super::Free();
}