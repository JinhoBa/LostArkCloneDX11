#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_Move final : public CPlayer_State
{
public:
	typedef struct PlayerMove_Desc
	{
		_float3 vPickingPosition;
	}PLAYERMOVE_DESC;

private:
	CPlayer_Move();
	virtual ~CPlayer_Move() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_float3 m_vPinkingPosition = {};

public:
	static CPlayer_Move* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
