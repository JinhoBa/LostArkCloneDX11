#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Idle final : public CMonster_State
{
private:
	CMonster_Idle();
	virtual ~CMonster_Idle() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CMonster_Idle* Create(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Free() override;
};

NS_END