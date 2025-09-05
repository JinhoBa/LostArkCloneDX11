#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_Dash final : public CPlayer_State
{
private:
	CPlayer_Dash();
	virtual ~CPlayer_Dash() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_bool m_bChangeStance = {};

public:
	static CPlayer_Dash* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
