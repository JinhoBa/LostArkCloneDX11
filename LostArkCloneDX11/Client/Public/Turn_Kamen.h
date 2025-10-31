#pragma once
#include "Client_Defines.h"
#include "State_Kamen.h"

NS_BEGIN(Client)

class CTurn_Kamen final : public CState_Kamen
{
public:
	typedef struct tagTurnKamenDesc
	{
		_uint iKamenStateID{};
	}TURN_KAMEN_DESC;

private:
	CTurn_Kamen();
	virtual ~CTurn_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_bool		m_isRight = {};
	_uint		m_iKamenStateID = {};
	_float		m_fDistance = {};

	_uint		m_iAttackCount = {};
	//_uint		m_StateID[12][ENUM_TO_INT(CKamen::KAMENSTATE::CLASH)];

public:
	static CTurn_Kamen* Create(STATE_KAMEN_DESC* pDesc);
	virtual void Free() override;
};

NS_END