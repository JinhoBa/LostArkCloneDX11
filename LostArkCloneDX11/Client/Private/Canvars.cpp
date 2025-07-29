#include "pch.h"
#include "Canvars.h"

#include "GameInstance.h"

CCanvars::CCanvars(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIPanel{pDevice, pContext}
{
}

CCanvars::CCanvars(CCanvars& Prototype)
    :CUIPanel{Prototype}
{
}

HRESULT CCanvars::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCanvars::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc = {};

    Desc.fX = g_iWinSizeX >> 1;
    Desc.fY = g_iWinSizeY >> 1;
    Desc.fSizeX = (_float)g_iWinSizeX;
    Desc.fSizeY = (_float)g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CCanvars::Priority_Update(_float fTimeDelta)
{
}

void CCanvars::Update(_float fTimeDelta)
{
}

void CCanvars::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CCanvars::Render()
{
    return S_OK;
}

HRESULT CCanvars::Add_Components()
{
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Canvars"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

CCanvars* CCanvars::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCanvars* pInstance = new CCanvars(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCanvars");
        return nullptr;
    }
    return pInstance;
}

CGameObject* CCanvars::Clone(void* pArg)
{
    CGameObject* pInstance = new CCanvars(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCanvars");
        return nullptr;
    }
    return pInstance;
}

void CCanvars::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
}
