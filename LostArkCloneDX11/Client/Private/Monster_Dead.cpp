#include "pch.h"
#include "Monster_Dead.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "StateMachine.h"

CMonster_Dead::CMonster_Dead()
	:CMonster_State{}
{
}

HRESULT CMonster_Dead::Initilize(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	if (FAILED(__super::Initilize(pStateMachine, pInfo, pMonster)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Dead::Enter(void* pArg)
{
	/* Dead Anim*/
	m_pMonster->Set_Animation(ANIMATIONSLOT::DEAD_LOOP);

	m_fTimeAcc = 0.f;
}

void CMonster_Dead::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(5.f < m_fTimeAcc)
		m_pMonster->Set_Dead(m_fTimeAcc);
}

void CMonster_Dead::Exit()
{

}

CMonster_Dead* CMonster_Dead::Create(CStateMachine* pStateMachine, ENEMY_INFO* pInfo, CMonster* pMonster)
{
	CMonster_Dead* pInstance = new CMonster_Dead();

	if (FAILED(pInstance->Initilize(pStateMachine, pInfo, pMonster)))
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