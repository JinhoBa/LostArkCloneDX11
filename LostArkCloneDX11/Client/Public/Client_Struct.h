#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"

namespace Client
{
	typedef struct tagSkill_Info
	{
		_bool			bCountAttack;
		_bool			bInvincible; // ����

		SKILL_TYPE		eSkilltype;
		PART_BREAKER	ePartbreaker; // ���� �ı�
		STAGGER			eStaggerLevel; // ����ȭ
		ATTACK_TYPE		eAttackType;
		SUPER_ARMOUR	eSuperArmour; // ���� �Ƹ�

		STANCE			eStance;
		_uint			iSkillID;
		
		_uint			iNeedMp;
		_uint			iNumAttack;
		_float			fCoolTime;
		_float			fIdenGauge; // �Ƶ� ���޷�

		string			strSkillName;
		wstring			strSkillNameKR;
		vector<_float>	Damages;
	}SKILL_INFO;

	typedef struct tagHit_Info
	{
		_float fDamage;
		_uint iHitType;
	}HIT_INFO;


	typedef struct tagPlayer_Info
	{
		_uint iHp;
		_uint iMp;
		_float fAttack;
	}PLAYER_INFO;

	typedef struct tagMapData
	{
		_wstring  strProtypeTag;
		_float3 vPosition;
		_float3 vScale;
		_float3 vRotation;
	}MAP_DATA;
}
