#include "UIBar.h"

#include "GameInstance.h"

CUIBar::CUIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUIBar::CUIBar(const CUIBar& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CUIBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIBar::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    UIBAR_DESC* pDesc = static_cast<UIBAR_DESC*>(pArg);

    m_fMax = pDesc->fMax;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_fY_Fill = -m_fY;
    m_fSizeY_Fill = m_fSizeY;

    m_iSRVIndex_Back = 0;
    m_iSRVIndex_Fill = 1;

    Update_Bar(pDesc->fStartValue);

    return S_OK;
}

void CUIBar::Priority_Update(_float fTimeDelta)
{

}

void CUIBar::Update(_float fTimeDelta)
{
}

void CUIBar::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIBar::Render()
{
    //// Render Back
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iSRVIndex_Back))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(1)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    // Render Fill
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransfromCom_BarFill->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iSRVIndex_Fill))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(1)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CUIBar::Update_Bar(_float fValue)
{
    _float Value = fValue;
    m_fSizeX_Fill = Value / m_fMax * m_fSizeX;

    m_fX_Fill = m_fX + (m_fSizeX_Fill * 0.5f - m_fSizeX * 0.5f);

    m_pTransfromCom_BarFill->Set_Scale(_float3(m_fSizeX_Fill, m_fSizeY_Fill, 1.f));

    _float4 vPosition = { m_fX_Fill, m_fY_Fill , m_fZ - 0.01f, 1.f };

    m_pTransfromCom_BarFill->Set_State(STATE::POSITION, XMLoadFloat4(&vPosition));

}

CUIBar* CUIBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIBar* pInstance = new CUIBar(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CUIBar");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CUIBar::Clone(void* pArg)
{
    CGameObject* pInstance = new CUIBar(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CUIBar");
        return nullptr;
    }

    return pInstance;
}

void CUIBar::Free()
{
    __super::Free();

    Safe_Release(m_pTransfromCom_BarFill);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    
}
