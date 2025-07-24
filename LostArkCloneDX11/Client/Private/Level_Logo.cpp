#include "Level_Logo.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) }
{
}

HRESULT CLevel_Logo::Initialize()
{
    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Logo::Render()
{
    SetWindowText(g_hWnd, TEXT("Logo"));

    return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLagerTag)
{
    return S_OK;
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
