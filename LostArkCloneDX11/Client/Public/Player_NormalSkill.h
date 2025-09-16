#pragma once
#include "Client_Defines.h"
#include "Player_State.h"

NS_BEGIN(Client)

class CPlayer_NormalSkill final : public CPlayer_State
{
public:
	typedef struct NormalSkill_Desc
	{
		_bool bLoop;
		_uint iAnimationIndex;
		_uint iSkillID;
	}NORMALSKILL_DESC;
private:
	CPlayer_NormalSkill();
	virtual ~CPlayer_NormalSkill() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	SKILL_INFO*		m_pSkillInfo = { nullptr };
	
	_bool			m_bStartHit = {};
	_bool			m_isActive = {};
	_uint			m_iNumAttack = {};
	_float			m_fTimeAcc = {};


public:
	static CPlayer_NormalSkill* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
