#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Base.h"

NS_BEGIN(Client)

class CSkill : public CBase
{
public:
	CSkill();
	virtual ~CSkill() = default;

public:
	void Set_PlayerInfo(PLAYER_INFO* pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	const _float Get_CoolTime() const { return m_fCoolTime; }
	const SKILL_INFO* Get_Skill_Info() { return m_pSkill_Info; }

public:
	HRESULT Initialize(_uint iSkillID);
	void Update(_float fTimeDelta);

	_bool Use_Skill();

private:
	_bool				m_bUse = {};
	_float				m_fCoolTime = {};
	PLAYER_INFO*		m_pPlayerInfo = { nullptr };

	const SKILL_INFO*	m_pSkill_Info = { nullptr };

public:
	static CSkill* Create(_uint iSkillID);
	virtual void Free() override;
};

NS_END