#include "pch.h"
#include "Monster_Dead.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Dead::CMonster_Dead()
	:CMonster_State{}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CMonster_Dead::Initilize(CStateMachine* pStateMachine, MONSTER* pType, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pType, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Dead::Enter(void* pArg)
{
	/* Dead Anim*/
	m_pMonster->Set_Animation(ANIMATIONSLOT::DEAD);
}

void CMonster_Dead::Update(_float fTimeDelta)
{
	/* Dead Loop Anim*/
	if (m_pMonster->isAnimationFinish())
		m_pMonster->Set_Animation(ANIMATIONSLOT::DEAD_LOOP);
}

void CMonster_Dead::Exit()
{

}

CMonster_Dead* CMonster_Dead::Create(CStateMachine* pStateMachine, MONSTER* pType, CMonster* pMonster)
{
	CMonster_Dead* pInstance = new CMonster_Dead();

	if (FAILED(pInstance->Initilize(pStateMachine, pType, pMonster)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Dead");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Dead::Free()
{
	__super::Free();
}