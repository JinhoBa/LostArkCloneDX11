#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CModel;
class CTransform;
class CGameInstance;
class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	virtual ~CState() = default;

public:
	void Set_State();

public:
	virtual HRESULT Initilize();
	virtual void Enter(void* pArg);
	virtual void Update(_float fTimeDelta);
	virtual void Exit();

protected:
	class CStateMachine*	m_pStateMachine = {};
	class CGameInstance*	m_pGameInstance = {};

public:
	virtual void Free() override;
};

NS_END