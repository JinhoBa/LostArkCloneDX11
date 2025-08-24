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
	vector<MAP_DATA>*		Get_MapDataPtr() { return &m_MapDatas; }
	vector<TERRAIN_DATA>*	Get_TerrainDataPtr() { return &m_TerrainDatas; }
	HRESULT					Load_MapData(const _char* pFilePath);
	HRESULT					Save_MapData(const _char* pFileName);
#pragma endregion

#pragma region MAP_PREVIEW
	vector<_wstring>*	Get_PreviewTexturesPtr() { return &m_MapPreviewFileNames; }
	HRESULT				Load_PreviewTextures(const _char* pFilePath);
#pragma endregion

#pragma region SKILL_DATA
	HRESULT				Load_SkillData(const _char* pFilePath);
	const SKILL_INFO*	Get_SkillInfo_Prt(_uint iSkillID);
#pragma endregion


private:
	CGameInstance*			m_pGameInstance = { nullptr };
	vector<TERRAIN_DATA>	m_TerrainDatas = {};
	vector<MAP_DATA>		m_MapDatas = {};
	vector<_wstring>		m_MapPreviewFileNames = {};
	vector<SKILL_INFO>		m_SkillDatas = {};
	
public:
	static CData_Manager* Creat();
	virtual void Free() override;
};

NS_END