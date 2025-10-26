#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Base.h"

NS_BEGIN(Engine)
class CCamera;
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);
private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	_vector Get_PickingPos() { return XMVectorSetW(XMLoadFloat3(&m_PickingPos),1.f); }

	void	Set_Level(LEVEL eLevel) { m_eCurLevel = eLevel; }
	LEVEL	Get_CurrentLevel() const { return m_eCurLevel; }
public:
	HRESULT Initialize_Manager();
	void Destory_GameManager();

	HRESULT Initialize_Effect_Manager();
	void	Add_Effect(EFFECT eType, _uint iEffectID, const _float4x4* pWorldMatirx, CHARACTER eCharacter = CHARACTER::PLAYER);
	void	Start_ScreenBlur(_float fTime);
	const vector<EFFECT_EVENT_DESC>& Get_EffectTrack(CHARACTER eType, _uint iTrackIndex);
	const vector<CAMERA_SHAKE_EVENT_DESC>& Get_Camera_Track(_uint iTrackIndex);
	const vector<BLUR_EVENT_DESC>& Get_BlurTrack(_uint iTrackIndex);

#ifdef _DEBUG
public:
	HRESULT Save_Effect(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID);
#endif // _DEBUG

#pragma region MAPDATA
	const vector<MAP_DATA>&		Get_MapData();
	const vector<TERRAIN_DATA>&	Get_TerrainData();

	HRESULT				Load_MapData(const _char* pMapDataFilePaht);
	HRESULT				Save_MapData(const _char* pFileName);
#pragma endregion

#pragma region SKILL_DATA
	HRESULT				Load_SkillData(const _char* pFilePath);
	SKILL_INFO*			Get_SkillInfo_Prt(_uint iSkillID);

	HRESULT				Load_Monster_SkillData(const _char* pFilePath);
	MONSTER_SKILL_INFO* Get_Monster_SkillInfo_Prt(_uint iMonsterID, _uint iSkillID);
#pragma endregion

#pragma region ANIMATION_DATA
	ANIMATION_DESC&		Get_AnimationIndex(_uint iMonsterID, ANIMATIONSLOT eSlot);
	HRESULT				Load_AnimationData(const _char* pFilePath);
#pragma endregion

#pragma region KAMEN_DATA
	MONSTER_SKILL_INFO* Get_KamenData(_uint iPhase, _uint iSkillID);
	HRESULT				Load_KamenData(const _char* pFilePath);
#pragma endregion

#pragma region CAMERA_ANIMATION
	const CAMERA_ANIMATION_DESC*	Get_CameraAnimation(_uint iID);
	HRESULT							Load_CameraAnimation(const _char* pFilePath);
#pragma endregion

#pragma region SKILL_MANAGER
	void			Set_PlayerInfoPrt(PLAYER_INFO* pPlayerInfo);
	void			Update_Skills(_float fTimeDelta);

	const _float	Check_CoolTime(_uint iSkillID) const;
	const _bool		Use_Skill(_uint iSkillID) const;
#pragma endregion

#pragma region BUFF_MANAGER
	class CBuff*	Add_Buff(_uint iBuffID);
	void			Remove_Buff(class CBuff* pBuff);
#pragma endregion

#pragma region DAMAGEFONT_MANAGER
	void	Add_DamageFont(DAMAGEFONT eType, _float fDamage, _float3& vPostion);
	void	Update_DamageFont(_float fTimeDelta);
#pragma endregion

#pragma region CLASH_MANAGER
	_uint	Update_Clash(_float fTimeDelta);
	_bool   isFinshed_Clash();
#pragma endregion

#pragma region DIALOGUE_CONTROLLER
	void	Start_Dialogue(_uint iNpcID, _fvector vNpcPosition);
	void	End_Dialogue();
#pragma endregion

	void Bind_PickingPos(_float3* pPickingPos);
	_vector Picking_Terrains();

private:
	_bool							m_bPicked = {};
	_float3							m_PickingPos = {};
	LEVEL							m_eCurLevel = {};

	CGameInstance*					m_pGameInstance = { nullptr };
	class CEffect_Manager*			m_pEffect_Manager = { nullptr };

	class CData_Manager*			m_pData_Manager = { nullptr };
	class CSkill_Manager*			m_pSkill_Manager = { nullptr };
	class CBuff_Manager*			m_pBuff_Manager = { nullptr };
	class CDamageFont_Manager*		m_pDamageFont_Manager = { nullptr };
	class CClash_Manager*			m_pClash_Manager = { nullptr };
	class CDialogue_Controller*		m_pDialogue_Controller = { nullptr };

public:
	virtual void Free() override;
};

NS_END