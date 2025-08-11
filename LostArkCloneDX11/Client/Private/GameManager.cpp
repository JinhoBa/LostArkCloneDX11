#include "pch.h"
#include "GameManager.h"

#include "Data_Manager.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{

}

HRESULT CGameManager::Initialize_Manager()
{
	m_pData_Manager = CData_Manager::Creat();
	if (nullptr == m_pData_Manager)
		return E_FAIL;

	return S_OK;
}

vector<MAP_DATA>* CGameManager::Get_MapData()
{
	return m_pData_Manager->Get_Data();
}

HRESULT CGameManager::Load_MapData(const _char* pMapDataFilePaht)
{
	m_pData_Manager->Load_File(pMapDataFilePaht);

	return S_OK;
}

HRESULT CGameManager::Save_MapData(const _char* pMapDataFilePaht)
{
	m_pData_Manager->Save_File(pMapDataFilePaht);

	return S_OK;
}

void CGameManager::Bind_PickingPos(_float3* pPickingPos)
{
	m_pPickingPos = pPickingPos;
}

void CGameManager::Free()
{
	__super::Free();

	Safe_Release(m_pData_Manager);
}
