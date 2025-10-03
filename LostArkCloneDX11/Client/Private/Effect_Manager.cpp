#include "pch.h"
#include "Effect_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"
#include "Monster.h"
#include "Kamen.h"
#include "Player.h"

CEffect_Manager::CEffect_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Manager::Initilize()
{

    return S_OK;
}


CEffect_Manager* CEffect_Manager::Create()
{
    CEffect_Manager* pInstance = new CEffect_Manager();

    if (FAILED(pInstance->Initilize()))
        return nullptr;

    return pInstance;
}

void CEffect_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
