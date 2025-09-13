#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_Jump final : public CPlayer_State
{
private:
	CPlayer_Jump();
	virtual ~CPlayer_Jump() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_float		m_fHeight = {};
	_float		m_fTimeAcc = {};
	_float		m_fRation = {};
	_float4		m_vTargetPosition = {};

public:
	static CPlayer_Jump* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
