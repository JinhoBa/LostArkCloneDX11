#include "Level_Loading.h"

#include "GameInstance.h"

#include "Loader.h"
#include "Level_Logo.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{pDevice, pContext, ENUM_TO_INT(eLevelID)}
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
    m_eNextLevelID = eNextLevelID;

    m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
    if (nullptr == m_pLoader)
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
    if (true == m_pLoader->isFinished() && GetKeyState(VK_RETURN))
    {
        CLevel* pNextLevel = { nullptr };

        switch (m_eNextLevelID)
        {
        case Client::LEVEL::LOGO:
            pNextLevel = CLevel_Logo::Create(m_pDevice, m_pContext, m_eNextLevelID);
            break;

        case Client::LEVEL::GAMEPLAY:
            break;
        }

        if(FAILED(m_pGameInstance->Change_Level(pNextLevel)))
            return;
    }
}

HRESULT CLevel_Loading::Render()
{
    return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _wstring& strLagerTag)
{
    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID, LEVEL eNextLevelID)
{
    CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize(eNextLevelID)))
    {
        MSG_BOX("Failed to Create : CLevel_Loading");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_Loading::Free()
{
    __super::Free();

    Safe_Release(m_pLoader);
}
