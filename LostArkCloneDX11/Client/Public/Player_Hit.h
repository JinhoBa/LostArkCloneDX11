#pragma once
#include "Client_Defines.h"
#include "State_Player.h"

NS_BEGIN(Client)

class CPlayer_Hit final : public CState_Player
{
	enum class STATE {HIT, DOWN, RECOVER, END};
public:
	typedef struct tagPlayerHitDesc
	{
		HIT_TYPE eType;
		_float3  vPosition;
	}PLAYER_HIT_DESC;
private:
	CPlayer_Hit();
	virtual ~CPlayer_Hit() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	HIT_TYPE	m_HitType = { HIT_TYPE::END };
	STATE		m_eState = { STATE::END };
	_uint		m_iNumAnimation = {};

	_float		m_fDownTime = {};

public:
	static CPlayer_Hit* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
