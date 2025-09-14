#pragma once
#include "Client_Defines.h"
#include "State_Kamen.h"

NS_BEGIN(Client)

class CIntro_Kamen final : public CState_Kamen
{
private:
	CIntro_Kamen();
	virtual ~CIntro_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_bool		m_bActive = {};
	_float		m_fTimeAcc = {};

public:
	static CIntro_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END