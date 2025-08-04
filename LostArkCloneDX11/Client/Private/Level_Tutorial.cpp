#include "pch.h"
#include "Level_Tutorial.h"
#include "GameInstance.h"
CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{pDevice, pContext, ENUM_TO_INT(eLevelID)}
{
}

HRESULT CLevel_Tutorial::Initialize()
{
    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;


    if (FAILED(Ready_Layer_UI(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_HUD(TEXT("Layer_HUD"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Tutorial::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Tutorial::Render()
{
    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    ///* Background */
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_Background_Logo"),
    //    ENUM_TO_INT(LEVEL::LOGO), strLayerTag)))
    //    return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UI(const _wstring& strLayerTag)
{
    /* UI */
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_HUD(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hpbar"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

CLevel_Tutorial* CLevel_Tutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
    CLevel_Tutorial* pInstance = new CLevel_Tutorial(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CLevel_Tutorial");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_Tutorial::Free()
{
    __super::Free();
}
