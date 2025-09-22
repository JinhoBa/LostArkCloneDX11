#pragma once
#include "Client_Defines.h"
#include "Skill_Player.h"

NS_BEGIN(Client)

class CPlayer_ChangeStance final : public CSkill_Player
{
private:
	CPlayer_ChangeStance();
	virtual ~CPlayer_ChangeStance() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CPlayer_ChangeStance* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
