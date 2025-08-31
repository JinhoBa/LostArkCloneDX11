#pragma once
#include "Client_Defines.h"

#include "Base.h"
#include "GameManager.h"
NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CData_Manager : public CBase
{
private:
	CData_Manager();
	virtual ~CData_Manager() = default;

public:
#pragma region MAP_DATA
	const vector<MAP_DATA>&		Get_MapData() { return m_MapDatas; }
	const vector<TERRAIN_DATA>&	Get_TerrainData() { return m_TerrainDatas; }

	HRESULT						Load_MapData(const _char* pFilePath);
	HRESULT						Save_MapData(const _char* pFileName);
#pragma endregion

#pragma region SKILL_DATA
	HRESULT						Load_SkillData(const _char* pFilePath);
	SKILL_INFO*			Get_SkillInfo_Prt(_uint iSkillID);
#pragma endregion

private:
	CGameInstance*			m_pGameInstance = { nullptr };

	vector<TERRAIN_DATA>	m_TerrainDatas = {};
	vector<MAP_DATA>		m_MapDatas = {};
	vector<_wstring>		m_MapPreviewFileNames = {};
	vector<SKILL_INFO>		m_SkillDatas = {};
	
public:
	static CData_Manager* Create();
	virtual void Free() override;
};

NS_END