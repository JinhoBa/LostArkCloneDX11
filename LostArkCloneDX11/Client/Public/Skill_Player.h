#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "State_Player.h"

NS_BEGIN(Client)

class CSkill_Player abstract : public CState_Player
{
protected:
	CSkill_Player();
	virtual ~CSkill_Player() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

	void Update_Hitbox(_float fTimeDelta);
	void Update_EffectTrack();

protected:
	_bool				m_isStartHit = {};
	_bool				m_isActiveHitBox = {};
	_uint				m_iSkillID = {};
	_uint				m_iAttackCount = {};
	_float				m_fTimeAcc = {};
	
	SKILL_INFO*			m_pSkillInfo = { nullptr };

	_float4x4*			m_pPlayerWorldMatrix = { nullptr };

	vector<EFFECT_EVENT> m_EffectEvents;


public:
	virtual void Free() override;
};

NS_END