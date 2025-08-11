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

vector<MAP_DATA>* CGameManager::Get_MapDataPtr()
{
	return m_pData_Manager->Get_MapDataPtr();
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

vector<_wstring>* CGameManager::Get_PreviewTexturesPtr()
{
	return m_pData_Manager->Get_PreviewTexturesPtr();
}

HRESULT CGameManager::Load_PreviewTextures(const _char* pFilePath)
{
	return m_pData_Manager->Load_PreviewTextures(pFilePath);
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
