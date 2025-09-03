#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_Idle final : public CPlayer_State
{
private:
	CPlayer_Idle();
	virtual ~CPlayer_Idle() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CPlayer_Idle* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END