#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

class CState_Player abstract : public CState
{
protected:
	CState_Player();
	virtual ~CState_Player() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

	_bool Check_Dash();

protected:
	class CGameManager* m_pGameManager = { nullptr };
	STANCE*				m_pPlayerStance = {nullptr};
	class CPlayer*		m_pPlayer = { nullptr };

public:
	virtual void Free() override;
};

NS_END