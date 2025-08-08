#pragma once
#include "Engine_Defines.h"

namespace Client
{
	typedef struct tagSkill_Info
	{
		_uint	iSkillID;
		string	strSkillName;
		_float	fDamage;
		_float	fCoolTime;
		_uint	iNeedMp;
		_uint	iNumAttack;

		_bool	bCountAttack;
		_uint	iStaggerLevel; // 무력화
		_bool   bSuperArmour;
		_bool	bInvincible; // 무적
		_uint   iAttackType;
		_float  fIdenGauge;
	}SKILL_INFO;

	typedef struct tagPlayer_Info
	{
		_uint iHp;
		_uint iMp;
		_float fAttack;
	};
}
