#pragma once
#include "Client_Defines.h"
#include "State_Player.h"

NS_BEGIN(Client)

class CPlayer_Clash final : public CState_Player
{
	enum class STATE {WAIT, START, LOOP, END};

private:
	CPlayer_Clash();
	virtual ~CPlayer_Clash() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	STATE	m_eState = {};

	_bool	m_isFinish = {};
	_float	m_fTimeAcc = {};

public:
	static CPlayer_Clash* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END