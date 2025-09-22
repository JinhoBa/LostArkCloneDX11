#pragma once
#include "Client_Defines.h"
#include "Skill_Player.h"

NS_BEGIN(Client)

class CPlayer_ComboSkill final : public CSkill_Player
{
public:
	typedef struct ComboSkill_Desc
	{
		_int iKey{};
		_uint iCount;
		_uint iAnimationIndex;
		_uint iSkillID;
	}COMBOSKILL_DESC;

private:
	CPlayer_ComboSkill();
	virtual ~CPlayer_ComboSkill() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	SKILL_INFO* m_pSkillInfo = { nullptr };
	
	_uint		m_iKey = {};
	_uint		m_iMaxCount = {};
	_uint		m_iCount = {};
	_uint		m_iAnimationIndex = {};

	_float		m_fTimeAcc = {};

public:
	static CPlayer_ComboSkill* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
