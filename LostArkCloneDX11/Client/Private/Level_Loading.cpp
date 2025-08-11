#include "pch.h"
#include "Level_Loading.h"

#include "GameInstance.h"

#include "Loader.h"
#include "Level_Logo.h"
#include "Level_Tutorial.h"
#include "Level_MapEditor.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{pDevice, pContext, ENUM_TO_INT(eLevelID)}
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
    m_eNextLevelID = eNextLevelID;

    m_fLoadingProgress = 1.f;

    m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
    if (nullptr == m_pLoader)
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;


    return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
    if (true == m_pLoader->isFinished() && m_pGameInstance->Get_KeyDown(DIK_RETURN))
    {
        CLevel* pNextLevel = { nullptr };

        switch (m_eNextLevelID)
        {
        case Client::LEVEL::LOGO:
            pNextLevel = CLevel_Logo::Create(m_pDevice, m_pContext, m_eNextLevelID);
            break;

        case Client::LEVEL::GAMEPLAY:
            break;

        case Client::LEVEL::TUTORIAL:
            pNextLevel = CLevel_Tutorial::Create(m_pDevice, m_pContext, m_eNextLevelID);
            break;

        case Client::LEVEL::MAP_EDITOR:
            pNextLevel = CLevel_MapEditor::Create(m_pDevice, m_pContext, m_eNextLevelID);
            break;
        }

        if(FAILED(m_pGameInstance->Change_Level(pNextLevel)))
            return;
    }
}

HRESULT CLevel_Loading::Render()
{
    m_fLoadingProgress = m_pLoader->Output();

    return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    /*Wallpaper*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Wallpaper"),
        ENUM_TO_INT(LEVEL::LOADING), strLayerTag, this)))
        return E_FAIL;

    /*Background*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Background_Loading"),
        ENUM_TO_INT(LEVEL::LOADING), strLayerTag)))
        return E_FAIL;

    /*Loading Bar*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_LoadingBar"),
        ENUM_TO_INT(LEVEL::LOADING), strLayerTag, this)))
        return E_FAIL;
    

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
