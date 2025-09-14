#pragma once
#include "Client_Defines.h"
#include "State_Kamen.h"

NS_BEGIN(Client)

class CIdle_Kamen final : public CState_Kamen
{
private:
	CIdle_Kamen();
	virtual ~CIdle_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_uint		m_iNumSkill = {};
	_uint		m_iSkillID = {};
	_uint		m_iAttack = {};
	
	_float		m_fTimeAcc = {};


public:
	static CIdle_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END