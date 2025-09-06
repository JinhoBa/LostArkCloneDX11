#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_ChargeSkill final : public CPlayer_State
{
private:
	enum CHARGE_STATE {START, LOOP, ATTACK, END};

public:
	typedef struct ChargeSkill_Desc
	{
		_int  iKey;
		_uint iSkillID;
	}CHARGESTATE_DESC;
private:
	CPlayer_ChargeSkill();
	virtual ~CPlayer_ChargeSkill() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	CHARGE_STATE	m_eState = {};
	
	_int			m_iKey = {};
	_uint			m_iSkillID = {};
	_uint			m_iAnimStart = {};
	_uint			m_iAnimLoop = {};
	_uint			m_iAnimEnd = {};

	_float			m_fChargeTime = {};
	_float			m_fMaxChargeTime = {};

	SKILL_INFO*		m_pSkillInfo = { nullptr };

public:
	static CPlayer_ChargeSkill* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
