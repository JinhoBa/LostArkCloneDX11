#pragma once
#include "Client_Defines.h"
#include "Attack_Kamen.h"

NS_BEGIN(Client)

class CAttack_Sword_Kamen final : public CAttack_Kamen
{
private:
	CAttack_Sword_Kamen();
	virtual ~CAttack_Sword_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

public:
	static CAttack_Sword_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END