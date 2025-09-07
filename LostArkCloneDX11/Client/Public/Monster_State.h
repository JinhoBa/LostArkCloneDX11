#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

class CMonster_State abstract : public CState
{
protected:
	CMonster_State();
	virtual ~CMonster_State() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, MONSTER* pType, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

protected:
	MONSTER*				m_pType = { nullptr };
	class CGameManager*		m_pGameManager = { nullptr };
	class CMonster*			m_pMonster = { nullptr };

public:
	virtual void Free() override;
};

NS_END