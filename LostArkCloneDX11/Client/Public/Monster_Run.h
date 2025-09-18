#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Run final : public CMonster_State
{
private:
	CMonster_Run();
	virtual ~CMonster_Run() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CMonster_Run* Create(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Free() override;
};

NS_END