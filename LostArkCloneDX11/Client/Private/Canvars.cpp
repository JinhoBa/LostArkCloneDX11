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
    Desc.fZ = 10.f;
    Desc.fSizeX = (_float)g_iWinSizeX;
    Desc.fSizeY = (_float)g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 1.f, 1.f));

#pragma region TEST_CODE
  /*  m_iTextureID = 0;
    m_fTimeAcc = 0.f;

    _float4x4 CameraWorldInv = {};
    _float4x4 OrthMatrix = {};

    XMStoreFloat4x4(&CameraWorldInv, XMMatrixIdentity());
    XMStoreFloat4x4(&OrthMatrix, (XMMatrixOrthographicLH(static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY), 0.1f, 100.f)));

    m_pShaderCom->SetMatrix(m_pTransformCom->Get_WorldMatrix(), CameraWorldInv, OrthMatrix);
    m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(0));*/
    
#pragma endregion

    return S_OK;
}

void CCanvars::Priority_Update(_float fTimeDelta)
{
}

void CCanvars::Update(_float fTimeDelta)
{
#pragma region TEST_CODE
    /*m_fTimeAcc += fTimeDelta;
    if (m_fTimeAcc >= 0.016f)
    {
        ++m_iTextureID;
        m_fTimeAcc = 0.f;

        if (m_iTextureID > 300)
            m_iTextureID = 0;

        m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(m_iTextureID));
    }*/
#pragma endregion
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
