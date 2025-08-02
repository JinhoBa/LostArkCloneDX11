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
    Desc.fZ = 1.f;
    Desc.fSizeX = (_float)g_iWinSizeX;
    Desc.fSizeY = (_float)g_iWinSizeY;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 1.f, 1.f));

#pragma region TEST_CODE
    _float4x4 CameraWorldInv = {};
    _float4x4 OrthMatrix = {};

    XMStoreFloat4x4(&CameraWorldInv, XMMatrixIdentity());
    XMStoreFloat4x4(&OrthMatrix, (XMMatrixOrthographicLH(static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY), 0.1f, 100.f)));

    m_pShaderCom->SetMatrix(m_pTransformCom->Get_WorldMatrix(), CameraWorldInv, OrthMatrix);
    m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(0));
    m_iTextureID = 0;
    m_fTimeAcc = 0.f;
#pragma endregion

    return S_OK;
}

void CCanvars::Priority_Update(_float fTimeDelta)
{
}

void CCanvars::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;
    if (m_fTimeAcc >= 0.016f)
    {
        ++m_iTextureID;
        m_fTimeAcc = 0.f;

        if (m_iTextureID > 300)
            m_iTextureID = 0;

        m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(m_iTextureID));
    }

}

void CCanvars::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CCanvars::Render()
{

    if (FAILED(m_pShaderCom->Apply()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources(m_pShaderCom->Get_Layout())))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;
    
    return S_OK;
}

HRESULT CCanvars::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Canvars"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
        TEXT("Com_Shader_VTXPosTex"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
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
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
