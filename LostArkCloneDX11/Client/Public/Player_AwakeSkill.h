#pragma once
#include "Client_Defines.h"
#include "Skill_Player.h"

NS_BEGIN(Client)

class CPlayer_AwakeSkill final : public CSkill_Player
{
public:
	typedef struct AwakeSkill_Desc
	{
		_bool bLoop;
		_uint iAnimationIndex;
		_uint iSkillID;
	}AWAKE_SKILL_DESC;
private:
	CPlayer_AwakeSkill();
	virtual ~CPlayer_AwakeSkill() = default;

public:
	virtual HRESULT Initilize(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Enter(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Exit()override;

private:
	_bool		m_isSpawWeaponEffect = { false };
	_float4		m_vPickingPosition = {};

private:
	void Play_SkillSound();


public:
	static CPlayer_AwakeSkill* Create(class CStateMachine* pStateMachine, STANCE* pStance, class CPlayer* pPlayer);
	virtual void Free() override;
};

NS_END
