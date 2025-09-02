#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CModel;
class CTransform;
class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	virtual ~CState() = default;

public:
	void Set_State(_uint iIndex, _bool bLoop = false);

public:
	virtual HRESULT Initilize(CModel* pModel, CTransform* pTransform);
	virtual void Enter();
	virtual void Update(_float fTimeDelta);
	virtual void Exit();

protected:
	_uint				m_iAnimIndex = {};
	_bool				m_bLoop = {};
	class CModel*		m_pModel = { nullptr };
	class CTransform*	m_pTransform = { nullptr };

public:
	virtual void Free() override;
};

NS_END