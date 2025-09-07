#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Turn final : public CMonster_State
{
private:
	CMonster_Turn();
	virtual ~CMonster_Turn() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CMonster_Turn* Create(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster);
	virtual void Free() override;
};

NS_END