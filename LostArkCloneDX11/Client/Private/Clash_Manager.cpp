#include "pch.h"
#include "Clash_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"
#include "Monster.h"
#include "Kamen.h"
#include "Player.h"

CClash_Manager::CClash_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() },
    m_pGameManager{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGameManager);
}

HRESULT CClash_Manager::Initilize()
{

    return S_OK;
}



void CClash_Manager::Update_Clash(_float fTimeDelta)
{
    
}

CClash_Manager* CClash_Manager::Create()
{
    CClash_Manager* pInstance = new CClash_Manager();

    if (FAILED(pInstance->Initilize()))
        return nullptr;

    return pInstance;
}

void CClash_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGameManager);
}
