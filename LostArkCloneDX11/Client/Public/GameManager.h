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
	CCamera* Get_Camera() { return m_pCamera; }

	void	Set_Camera(CCamera* pCamera);
	void	Set_Level(LEVEL eLevel) { m_eCurLevel = eLevel; }
	LEVEL	Get_CurrentLevel() const { return m_eCurLevel; }
public:
	HRESULT Initialize_Manager();

#pragma region MAPDATA
	const vector<MAP_DATA>&		Get_MapData();
	const vector<TERRAIN_DATA>&	Get_TerrainData();

	HRESULT				Load_MapData(const _char* pMapDataFilePaht);
	HRESULT				Save_MapData(const _char* pFileName);
#pragma endregion

#pragma region SKILL_DATA
	HRESULT				Load_SkillData(const _char* pFilePath);
	SKILL_INFO*			Get_SkillInfo_Prt(_uint iSkillID);
#pragma endregion

#pragma region ANIMATION_DATA
	ANIMATION_DESC&		Get_AnimationIndex(_uint iMonsterID, ANIMATIONSLOT eSlot);
	HRESULT				Load_AnimationData(const _char* pFilePath);
#pragma endregion

#pragma region KAMEN_DATA
	const vector<KAMEN_SKILL_DESC>& Get_KamenData(_uint iPhase);

	HRESULT			Load_KamenData(const _char* pFilePath);
#pragma endregion


#pragma region SKILL_MANAGER
	void		Set_PlayerInfoPrt(PLAYER_INFO* pPlayerInfo);
	void		Update_Skills(_float fTimeDelta);

	const _float		Check_CoolTime(_uint iSkillID) const;
	const _bool			Use_Skill(_uint iSkillID) const;
#pragma endregion

#pragma region BUFF_MANAGER
	class CBuff* Add_Buff(_uint iBuffID);
	void Remove_Buff(class CBuff* pBuff);
#pragma endregion

#pragma region DAMAGEFONT_MANAGER
	void Add_DamageFont(DAMAGEFONT eType, _float fDamage, _float3& vPostion);
	void Update_DamageFont(_float fTimeDelta);
#pragma endregion


	void Bind_PickingPos(_float3* pPickingPos);
	_vector Picking_Terrains();
	

private:
	_bool							m_bPicked = {};
	_float3							m_PickingPos = {};
	LEVEL							m_eCurLevel = {};

	CGameInstance*					m_pGameInstance = { nullptr };
	class CCamera*					m_pCamera = { nullptr };

	class CData_Manager*			m_pData_Manager = { nullptr };
	class CSkill_Manager*			m_pSkill_Manager = { nullptr };
	class CBuff_Manager*			m_pBuff_Manager = { nullptr };
	class CDamageFont_Manager*		m_pDamageFont_Manager = { nullptr };

public:
	virtual void Free() override;
};

NS_END