#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"

namespace Client
{
	typedef struct tagHitBoxDesc
	{
		_float fDamage;
		_float fStartTime;
		_float fDuration;
		_float fInterval;
		_float3 vOffset;
		_float3 vExtends;
	}HITBOX_DESC;

	typedef struct tagSkill_Info
	{
		_bool			bCountAttack;
		_bool			bInvincible; // 무적
		_bool			bApplyRimLightBody;
		_bool			bApplyRimLightWeapon;

		SKILL_TYPE		eSkilltype;
		PART_BREAKER	ePartbreaker; // 부위 파괴
		STAGGER			eStaggerLevel; // 무력화
		ATTACK_TYPE		eAttackType;
		HIT_TYPE		eHitType;
		SUPER_ARMOUR	eSuperArmour; // 슈퍼 아머

		STANCE			eStance;
		_uint			iSkillID;
		
		_uint			iNeedMp;
		_uint			iNumAttack;
		_float			fCoolTime;
		_float			fIdenGauge; // 아덴 수급량

		HITBOX_DESC     HitBoxDesc;

		string			strSkillName;
		wstring			strSkillNameKR;
		vector<_float>	Damages;
	}SKILL_INFO;

	typedef struct tagMonsterSkill_Info
	{
		_uint				iSkillID;
		HIT_TYPE			eHitType;
		_uint				iNumAttack;
		vector<HITBOX_DESC> HitBoxDescs;
	}MONSTER_SKILL_INFO;

	typedef struct tagAttack_Desc
	{
		_bool			isCritial;
		_float			fDamage;
		ATTACK_TYPE		eAttackType;
		HIT_TYPE		eHitType;
	}ATTACK_DESC;

	typedef struct tagCharacter_Info
	{
		_float fMaxHp;
		_float fHp;
		_float fAttack;
	}CHARACTER_INFO;

	typedef struct tagPlayer_Info : public CHARACTER_INFO
	{
		STANCE eStance;
		_float fMaxMp;
		_float fMp;
		_float fIdentity;
		_float Critical_Probability;
		_float Critical_Damage;
		_float fAttackSpeed;
		_float fMoveSpeed;
	}PLAYER_INFO;

	typedef struct tagEnemy_Info : public CHARACTER_INFO
	{
		_uint  iMonsterID;
		_float fDetectDistance;
		_float fAttackRange;
	}ENEMY_INFO;

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
		vector<MONSTER_SKILL_INFO> Skills;
	}KAMEN_PHASE_DESC;

	typedef struct tagCameraAnimationDesc
	{
		_bool		isLoop{};
		_bool		isRotate{};
		_float		fDuration{};
		_float      fSpeed{};
		_float		fFov{};
		_float      fRotationSpeed{};
		_float3		vStartPositon{};
		_float3		vEndPosition{};
		_float3     vRotationAxis{};
		_float3		vTargetPosition{};
	}CAMERA_ANIMATION_DESC;

	typedef struct tagEffectDesc
	{
		EFFECT	eType;
		LERP	eLerpType;
		_bool	isLoop;
		_bool	isMove;
		_bool	isRotate;
		_float	fRotationSpeed;
		_float3 vStrartPosition;
		_float3 vEndPosition;
		_float3 vRotation;
		_float3 vScale;
		_float2 vLifeTime;
		
	};

	typedef struct EffectGroundDesc
	{
		LERP		eLerpType = { LERP::LINEAR };
		_bool		isRotation = {};
		_uint		iID = {};
		_uint		iBaseIndex = {};
		_uint		iMaskIndex = {};
		_uint		iNoiseIndex = {};

		_float		fSpeed = {};
		_float		fLifeTime = {};
		_float		fRotationSpeed = {};

		_float2		vSize = {};
		_float3		vPosition = {};
		_float3		vPivot = {};
		_float3		vRotation = {};
	}EFFECT_GROUND_DESC;

	typedef struct tagEffectMeshData
	{
		_bool		isLoop = {};
		_bool		isUseScale = {};
		_bool		isUseRotation = {};

		_uint		iMeshIndex = {};

		_float		fSpeed = {};
		_float3		vPosition = {};
		_float3		vPivot = {};

		_float3		vStartRotation = {};
		_float3		vEndRotation = {};

		_float3		vStartScale = {};
		_float3		vEndScale = {};

		_float2		vLifeTime = {};

		_uint       iPassIndex = {};

		_float		fDiffuseScrollSpeedU = {};
		_float		fDiffuseScrollSpeedV = {};

		_float		fMaskScrollSpeedU = {};
		_float		fMaskScrollSpeedV = {};
	}EFFECT_MESH_DATA;

	typedef struct tagEffectMeshDesc
	{
		EFFECT_MESH_DATA Mesh_Data = {};

		_wstring	strDiffuseTexture = {};
		_wstring	strMaskTexture = {};
		_wstring	strNoiseTexture = {};
	}EFFECT_MESH_DESC;

	typedef struct tagEffecEventDesc
	{
		EFFECT	eType;
		_uint	iID;
		_float	fKeyFrame;
	}EFFECT_EVENT_DESC;

	typedef struct tagEffecEvent
	{
		_bool isTrigge;
		EFFECT_EVENT_DESC EventDesc;
	}EFFECT_EVENT;
}
