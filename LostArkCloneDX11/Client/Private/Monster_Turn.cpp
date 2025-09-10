#include "pch.h"
#include "Monster_Turn.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Turn::CMonster_Turn()
	:CMonster_State{}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_Turn::Initilize(CStateMachine* pStateMachine, MONSTER* pType, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pType, pMonster)))
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
}

void CMonster_Turn::Update(_float fTimeDelta)
{
	if (false == m_pMonster->Turn(fTimeDelta))
	{
		m_pStateMachine->Change_State(m_pMonster->Get_State(CMonster::STATE::ATTACK), nullptr);
	}
}

void CMonster_Turn::Exit()
{

}

CMonster_Turn* CMonster_Turn::Create(CStateMachine* pStateMachine, MONSTER* pType, CMonster* pMonster)
{
	CMonster_Turn* pInstance = new CMonster_Turn();

	if (FAILED(pInstance->Initilize(pStateMachine, pType, pMonster)))
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