#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CCamera;
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);
private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	_float3* Get_PickingPos() { return &m_PickingPos; }
	CCamera* Get_Camera() { return m_pCamera; }

	void Set_Camera(CCamera* pCamera);
	void Set_Level(LEVEL eLevel) { m_eCurLevel = eLevel; }
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


	void Bind_PickingPos(_float3* pPickingPos);
	_vector Picking_Terrains();
	

private:
	_float3					m_PickingPos = {};
	LEVEL					m_eCurLevel = {};

	CGameInstance*			m_pGameInstance = { nullptr };
	class CCamera*			m_pCamera = { nullptr };

	class CData_Manager*	m_pData_Manager = { nullptr };
	class CSkill_Manager*	m_pSkill_Manager = { nullptr };
	class CBuff_Manager*	m_pBuff_Manager = { nullptr };

public:
	virtual void Free() override;
};

NS_END