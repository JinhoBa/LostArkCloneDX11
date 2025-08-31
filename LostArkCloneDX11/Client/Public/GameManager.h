#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);
private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	_float3* Get_PickingPos() { return m_pPickingPos; }

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
	void				Update_Skills(_float fTimeDelta);

	const _float		Check_CoolTime(_uint iSkillID) const;
	const _bool			Use_Skill(_uint iSkillID) const;
#pragma endregion


	void Bind_PickingPos(_float3* pPickingPos);
	

private:
	_float3*				m_pPickingPos = {};

	class CData_Manager*	m_pData_Manager = { nullptr };
	class CSkill_Manager*	m_pSkill_Manager = { nullptr };

public:
	virtual void Free() override;
};

NS_END