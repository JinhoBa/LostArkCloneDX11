#pragma once
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

class CState_Kamen abstract : public CState
{
public:
	typedef struct State_Kamen_Desc
	{
		class CStateMachine*	pStateMachine{ nullptr };
		const PHASE*			pPhase{ nullptr };
		class CKamen*			pKamen{ nullptr };
	}STATE_KAMEN_DESC;
protected:
	CState_Kamen();
	virtual ~CState_Kamen() = default;

public:
	virtual HRESULT Initilize(STATE_KAMEN_DESC* pDesc);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

protected:
	_uint					m_iEffectID = {};

	class CGameManager*		m_pGameManager = { nullptr };

	const PHASE*			m_pPhase = { nullptr };
	class CKamen*			m_pKamen = { nullptr };
	_float4x4*				m_pKamenWorldMatrix = {};

	vector<EFFECT_EVENT>	m_EffectEvents;

protected:
	void Ready_EffectEvents();
	void Update_EffectTrack();

public:
	virtual void Free() override;
};

NS_END