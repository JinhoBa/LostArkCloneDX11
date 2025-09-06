#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CSkill_Manager : public CBase
{
private:
	CSkill_Manager();
	virtual ~CSkill_Manager() = default;

public:
	void	Set_PlayerInfoPrt(PLAYER_INFO* pPlayerInfo);
	const _float Check_CoolTime(_uint iSkillID) const;
	const _bool Use_Skill(_uint iSkillID) const;

public:
	HRESULT		Initialize();
	void		Update(_float fTimeDelta);

private:
	vector<class CSkill*> m_Skills;

public:
	static CSkill_Manager* Create();
	virtual void Free() override;
};

NS_END