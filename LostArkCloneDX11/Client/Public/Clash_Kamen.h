#pragma once
#include "Client_Defines.h"
#include "State_Kamen.h"

NS_BEGIN(Client)

class CClash_Kamen final : public CState_Kamen
{
private:
	CClash_Kamen();
	virtual ~CClash_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc, class CTransform* pPlayerTransform);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	class CTransform* m_pPlayerTransform = { nullptr };

private:
	void Enter_Clash();

public:
	static CClash_Kamen* Create(STATE_KAMEN_DESC* pDesc, class CTransform* pPlayerTransform);
	virtual void Free() override;
};

NS_END