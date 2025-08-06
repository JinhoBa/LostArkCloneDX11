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

    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;
    m_fZ = 10.f;
    m_fSizeX = (_float)g_iWinSizeX;
    m_fSizeY = (_float)g_iWinSizeY;

    if (FAILED(CGameObject::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 1.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));

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
}

HRESULT CCanvars::Render()
{
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
}
