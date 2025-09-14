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
	_uint				m_iSkillID = {};
	KAMEN_SKILL_DESC	m_SkillDesc = {};
public:
	virtual void Free() override;
};

NS_END