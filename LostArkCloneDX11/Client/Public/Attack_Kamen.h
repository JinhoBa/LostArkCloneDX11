#pragma once
#include "Client_Defines.h"
#include "State_Kamen.h"

NS_BEGIN(Client)

class CAttack_Kamen abstract : public CState_Kamen
{
public:
	typedef struct Attack_Kamen_Desc
	{
		_uint iSkillID{};
	}ATTACK_KAMEN_DESC;

protected:
	CAttack_Kamen();
	virtual ~CAttack_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;
	
protected:
	COLLIDER			m_eHitboxType = {};
	_bool				m_isStartHit = {};
	_bool				m_isActiveHitBox = {};
	_uint				m_iSkillID = {};
	_uint				m_iAttackCount = {};
	_float				m_fTimeAcc = {};

	class CTransform*	m_pPlayerTransform = { nullptr };

	MONSTER_SKILL_INFO*	m_pSkillDesc = {};

protected:
	void Update_HitBox(_float fTimeDelta);

public:
	virtual void Free() override;
};

NS_END