#include "pch.h"
#include "Level_Logo.h"

#include "GameInstance.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) }
{
}

HRESULT CLevel_Logo::Initialize()
{
    m_fBGM_Volume = 0.5f;

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
}

HRESULT CLevel_Logo::Render()
{
    SetWindowText(g_hWnd, TEXT("Logo"));

    return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLagerTag)
{
    /*Add Canvars*/
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_Canvars"),
        ENUM_TO_INT(LEVEL::LOGO), strLagerTag)))
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
