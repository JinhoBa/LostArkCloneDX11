#pragma once
#include "Client_Defines.h"
#include "Attack_Kamen.h"

NS_BEGIN(Client)

class CAttack_Combo_Kamen final : public CAttack_Kamen
{
	enum class STATE {READY, START, LOOP, ATTACK, RECOVER, END };
private:
	CAttack_Combo_Kamen();
	virtual ~CAttack_Combo_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	STATE		m_eState = {};
	_bool		m_Triggers[3];
	_float      m_KeyFrames[3];

private:
	void Update_HitRange(_float fTimeDelta);

public:
	static CAttack_Combo_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END