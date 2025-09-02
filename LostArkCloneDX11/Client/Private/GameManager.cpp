#include "pch.h"
#include "GameManager.h"

#include "Data_Manager.h"
#include "Skill_Manager.h"

#include "UIObject.h"
#include "Camera_Fix.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{

}

void CGameManager::Set_Camera(CCamera* pCamera)
{
	if (nullptr != m_pCamera)
		Safe_Release(m_pCamera);

	m_pCamera = pCamera;
	Safe_AddRef(m_pCamera);
}

HRESULT CGameManager::Initialize_Manager()
{
	m_pData_Manager = CData_Manager::Create();
	if (nullptr == m_pData_Manager)
		return E_FAIL;

	if (FAILED(m_pData_Manager->Load_SkillData("../Bin/Resources/Data/Skill_Data.xml")))
		return E_FAIL;

	m_pSkill_Manager = CSkill_Manager::Create();
	if (nullptr == m_pSkill_Manager)
		return E_FAIL;

	return S_OK;
}


#pragma region DATA_MANAGER

const vector<MAP_DATA>& CGameManager::Get_MapData()
{
	return m_pData_Manager->Get_MapData();
}

const vector<TERRAIN_DATA>& CGameManager::Get_TerrainData()
{
	return m_pData_Manager->Get_TerrainData();
}

HRESULT CGameManager::Load_MapData(const _char* pMapDataFilePaht)
{
	m_pData_Manager->Load_MapData(pMapDataFilePaht);

	return S_OK;
}

HRESULT CGameManager::Save_MapData(const _char* pFileName)
{
	m_pData_Manager->Save_MapData(pFileName);

	return S_OK;
}

HRESULT	 CGameManager::Load_SkillData(const _char* pFilePath)
{
	return m_pData_Manager->Load_SkillData(pFilePath);
}

SKILL_INFO* CGameManager::Get_SkillInfo_Prt(_uint iSkillID)
{
	return m_pData_Manager->Get_SkillInfo_Prt(iSkillID);
}

#pragma endregion

#pragma region SKILL_MANAGER

void CGameManager::Update_Skills(_float fTimeDelta)
{
	m_pSkill_Manager->Update(fTimeDelta);
}

const _float CGameManager::Check_CoolTime(_uint iSkillID) const
{
	if (99 == iSkillID)
		return 0.f;
	return m_pSkill_Manager->Check_CoolTime(iSkillID);
}

const _bool	CGameManager::Use_Skill(_uint iSkillID) const
{
	return m_pSkill_Manager->Use_Skill(iSkillID);
}

#pragma endregion

void CGameManager::Bind_PickingPos(_float3* pPickingPos)
{
	if (nullptr == m_pPickingPos)
		m_pPickingPos = pPickingPos;
	//if(m_pPickingPos->y <= pPickingPos->y)
	//	m_pPickingPos = pPickingPos;
}

void CGameManager::Free()
{
	__super::Free();

	Safe_Release(m_pCamera);
	Safe_Release(m_pData_Manager);
	Safe_Release(m_pSkill_Manager);
}
