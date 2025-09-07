#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Attack final : public CMonster_State
{
private:
	CMonster_Attack();
	virtual ~CMonster_Attack() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster, _uint iNumAttackAnimation);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_uint					m_iAttackIndex = {};
	_uint					m_iNumAttackAnimation = {};

public:
	static CMonster_Attack* Create(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster, _uint iNumAttackAnimation);
	virtual void Free() override;
};

NS_END