#include "pch.h"
#include "MainApp.h"

#include "GameInstance.h"

#include <fstream>

#include "Level_Loading.h"
#include "Canvars.h"

CMainApp::CMainApp()
    : m_pGameInstance{ CGameInstance::GetInstance() }, 
    m_iFps{ 0 }, m_fTimeAcc{ 0.f }, m_iFrame{0}
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

    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Start_Level(LEVEL::LOGO)))
        return E_FAIL;

#pragma region IMGUI INIT
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImFont* font = nullptr;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    string strFontTag = "../Bin/Resources/Fonts/NanumMyeongjo.TTF";

    ifstream ifile;

    ifile.open(strFontTag);
    if (ifile)
    {
        font = io.Fonts->AddFontFromFileTTF(strFontTag.c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesKorean());
    }
    IM_ASSERT(font != NULL);
    ifile.close();
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);
#pragma endregion

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
    m_pGameInstance->Update_Engine(fTimeDelta);

    m_fTimeAcc += fTimeDelta;
    ++m_iFrame;
    if (1.f <= m_fTimeAcc)
    {
        m_iFps = m_iFrame;
        m_iFrame = 0;
        m_fTimeAcc = 0.f;
    }
    
}

HRESULT CMainApp::Render()
{
    _float4 vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

    m_pGameInstance->Render_Begin(&vClearColor);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_pGameInstance->Draw();

    

    ImGui::Begin("FPS");
    ImGui::Text("%d", m_iFps);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_pGameInstance->Render_End();

    return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
    if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, eLevelID))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMainApp::Ready_Prototype()
{
    /*For Prototype_Component_Transform*/
    if(FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        CTransform::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    /*For Prototype_Component_VIBuffer_Rect*/
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    D3D11_INPUT_ELEMENT_DESC		Elements[] = {
        	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}		
        };

    /*For Prototype_Component_Transform*/
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
        CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTXPosTex.hlsl"), Elements, 2))))
        return E_FAIL;
    
    /*For Prototype_Component_Transform*/
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_UIAnimation"),
        CUIAnimation::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    /*For Prototype_GameObject_Canvars*/
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Canvars"),
        CCanvars::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    /*Add Canvars*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Canvars"),
        ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"))))
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

#pragma region IMGUI_RELEASE
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#pragma endregion

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pGameInstance->Release_Engine();

    Safe_Release(m_pGameInstance);
}
