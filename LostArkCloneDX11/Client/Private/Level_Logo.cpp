#include "pch.h"
#include "Level_Logo.h"

#include "GameInstance.h"

#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) }
{
}

HRESULT CLevel_Logo::Initialize()
{
    m_fBGM_Volume = 0.1f;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

#pragma region TEST_CODE
    m_pGameInstance->PlayBGM(L"vol3_05_Bern Castle.mp3", m_fBGM_Volume);
#pragma endregion

    SetWindowText(g_hWnd,TEXT("LEVEL_LOGO"));

    return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
    m_pGameInstance->SetChannelVolume(CHANNELID::BGM, m_fBGM_Volume);

    if (m_pGameInstance->Get_KeyDown(DIK_N))
    {
        m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, LEVEL::TUTORIAL));
    }
    else if (m_pGameInstance->Get_KeyDown(DIK_M))
    {
        m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, LEVEL::MAP_EDITOR));
    }
}

HRESULT CLevel_Logo::Render()
{
    SetWindowText(g_hWnd, TEXT("Logo"));

    return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    /* Background */
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_Background_Logo"), 
        ENUM_TO_INT(LEVEL::LOGO), strLayerTag)))
        return E_FAIL;

    /*ServerListPanel*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoPanel"),
        ENUM_TO_INT(LEVEL::LOGO), strLayerTag)))
        return E_FAIL;

    /*ServerListPanel*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_ServerListPanel"), 
        ENUM_TO_INT(LEVEL::LOGO), strLayerTag)))
        return E_FAIL;
  

    return S_OK;
}

void CLevel_Logo::Update_GUI()
{
    ImGui::SliderFloat("BGM Volume :  ",&m_fBGM_Volume, 0.f, 1.f);
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
    CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CLevel_Logo");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_Logo::Free()
{
    __super::Free();
}
