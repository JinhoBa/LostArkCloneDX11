#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"

namespace Client
{
	typedef struct tagSkill_Info
	{
		_bool			bCountAttack;
		_bool			bInvincible; // 무적

		SKILL_TYPE		eSkilltype;
		PART_BREAKER	ePartbreaker; // 부위 파괴
		STAGGER			eStaggerLevel; // 무력화
		ATTACK_TYPE		eAttackType;
		SUPER_ARMOUR	eSuperArmour; // 슈퍼 아머

		STANCE			eStance;
		_uint			iSkillID;
		
		_uint			iNeedMp;
		_uint			iNumAttack;
		_float			fCoolTime;
		_float			fIdenGauge; // 아덴 수급량

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
		STANCE eStance;
		_float fMaxHp;
		_float fMaxMp;
		_float fHp;
		_float fMp;
		_float fAttack;
		_float fIdentity;
		_float Critical_Probability;
		_float Critical_Damage;
		_float fAttackSpeed;
		_float fMoveSpeed;
	}PLAYER_INFO;

	typedef struct tagMonster_Info
	{
		_float fMaxHp;
		_float fHp;
		_float fAttack;
		_float fDetectDistance;
		_float fAttackRange;
	}MONSTER_INFO;

	typedef struct tagStatistics
	{
		_float fAtk_Flat;
		_float fAtk_Pct;
		_float fAtkSpeed_Pct;
		_float fMoveSpeed_Pct;
		_float Critical_Probability_Flat;
		_float Critical_Damage_Pct;
	}BUFFSTAT;

	typedef struct tagChargeSkill_Desc
	{
		_bool	isUsing;
		_float	fMaxChargeTime;
		_float	fChargingTime;
	}CHARGE_SKILL_DESC;

	typedef struct tagMapData
	{
		_wstring strPrototypeTag;
		_float3 vPosition;
		_float3 vScale;
		_float3 vRotation;
	}MAP_DATA;

	typedef struct tagTerrainData
	{
		_wstring strPrototypeTag;
		_float3 vPosition;
		_float3 vRotation;
	}TERRAIN_DATA;

	typedef struct tagAnimationDesc
	{
		ANIMATIONSLOT eSlot;
		_uint iIndex;
		_bool bLoop;
		_float fLerpTime;
	}ANIMATION_DESC;

	typedef struct tagKamenSkillDesc
	{
		_uint iNumAnimation;

		_int iAnimationIndexStart;
		_int iAnimationIndexLoop;
		_int iAnimationIndexEnd;

		_float fDamage;
		_float fLoopTime;

	}KAMEN_SKILL_DESC;

	typedef struct tagKamenPhaseDesc
	{
		_uint	iPhaseID;
		_uint	iNumSkill;
		_float	fConditionValue;
		vector<KAMEN_SKILL_DESC> Skills;
	}KAMEN_PHASE_DESC;

	
}
