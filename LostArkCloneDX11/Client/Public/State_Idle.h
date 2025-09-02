#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

class CState_Idle final : public CState
{
private:
	CState_Idle();
	virtual ~CState_Idle() = default;

public:
	virtual HRESULT Initilize(CModel* pModel, CTransform* pTransform)override;
	virtual void Enter()override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;


public:
	static CState_Idle* Create(class CModel* pModel, class CTransform* pTransform);
	virtual void Free() override;
};

NS_END