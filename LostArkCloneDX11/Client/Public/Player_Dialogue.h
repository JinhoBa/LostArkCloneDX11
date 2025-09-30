#pragma once
#include "Client_Defines.h"
#include "State_Player.h"

NS_BEGIN(Client)

class CPlayer_Dialogue final : public CState_Player
{
public:
	typedef struct tagPlayerDialogueDesc
	{
		_float3		vTargetPosition;
		_float3		vPosition;
	}PLAYER_DIALOGUE_DESC;

private:
	CPlayer_Dialogue();
	virtual ~CPlayer_Dialogue() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_float4		m_vPrePosition = {};

public:
	static CPlayer_Dialogue* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
