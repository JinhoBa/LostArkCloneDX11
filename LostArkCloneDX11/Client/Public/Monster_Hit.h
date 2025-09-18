#pragma once
#include "Client_Defines.h"
#include "Monster_State.h"

NS_BEGIN(Client)

class CMonster_Hit final : public CMonster_State
{
	enum class STATE { HIT, LAND, STANDUP, END};
public:
	typedef struct Monster_Hit_Desc
	{
		HIT_TYPE eHitType;
	}MONSTER_HIT_DESC;

private:
	CMonster_Hit();
	virtual ~CMonster_Hit() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	CMonster_Hit::STATE		m_eState = {};
	HIT_TYPE				m_eHitType = {};

	_float					m_fStiffnessTime = {};

public:
	static CMonster_Hit* Create(class CStateMachine* pStateMachine, ENEMY_INFO* pInfo, class CMonster* pMonster);
	virtual void Free() override;
};

NS_END