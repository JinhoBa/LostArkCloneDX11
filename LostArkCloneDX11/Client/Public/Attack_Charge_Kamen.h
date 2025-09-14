#pragma once
#include "Client_Defines.h"
#include "Attack_Kamen.h"

NS_BEGIN(Client)

class CAttack_Charge_Kamen final : public CAttack_Kamen
{
	enum class STATE {START, LOOP, END};
private:
	CAttack_Charge_Kamen();
	virtual ~CAttack_Charge_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	STATE		m_eState = {};

	_float		m_fTimeAcc = {};

public:
	static CAttack_Charge_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END