#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Attack final : public CMonster_State
{
private:
	CMonster_Attack();
	virtual ~CMonster_Attack() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster, _uint iNumAttackAnimation);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_bool						m_isActive = {};
	_uint						m_iNumHit = {};
	_uint						m_iAttackIndex = {};
	_uint						m_iNumAttackAnimation = {};

	_float						m_fTimeAcc = { 0.f };

	MONSTER_SKILL_INFO*   m_pSkill_Info = { nullptr };

private:
	void Play_Sound();

public:
	static CMonster_Attack* Create(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster, _uint iNumAttackAnimation);
	virtual void Free() override;
};

NS_END