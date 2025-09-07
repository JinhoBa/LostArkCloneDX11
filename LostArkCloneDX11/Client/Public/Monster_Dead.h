#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Dead final : public CMonster_State
{
private:
	CMonster_Dead();
	virtual ~CMonster_Dead() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CMonster_Dead* Create(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster);
	virtual void Free() override;
};

NS_END