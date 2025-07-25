#include "pch.h"
#include "MainApp.h"

#include "GameInstance.h"

#include "Level_Loading.h"

CMainApp::CMainApp()
    : m_pGameInstance{CGameInstance::GetInstance()}
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
    ENGINE_DESC				EngineDesc{};
    EngineDesc.hInstance = g_hInstance;
    EngineDesc.hWnd = g_hWnd;
    EngineDesc.eWindowMode = WINMODE::WIN;
    EngineDesc.iWinSizeX = g_iWinSizeX;
    EngineDesc.iWinSizeY = g_iWinSizeY;
    EngineDesc.iNumLevels = ENUM_TO_INT(LEVEL::END);

    if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
        return E_FAIL;

    if (FAILED(Start_Level(LEVEL::LOGO)))
        return E_FAIL;

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
    m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
    _float4 vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

    m_pGameInstance->Render_Begin(&vClearColor);

    m_pGameInstance->Draw();

    m_pGameInstance->Render_End();

    return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
    if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, eLevelID))))
        return E_FAIL;

    return S_OK;
}

CMainApp* CMainApp::Create()
{
    CMainApp* pInstance = new CMainApp();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CMainGame");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CMainApp::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pGameInstance->Release_Engine();

    Safe_Release(m_pGameInstance);
}
