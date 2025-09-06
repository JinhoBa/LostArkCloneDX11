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
    m_fValue = pDesc->fStartValue;
   
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    _float3 vPos = {};
    XMStoreFloat3(&vPos, m_pParent_TransformCom->Get_Position());

    m_iSRVIndex_Back = 0;
    m_iSRVIndex_Fill = 1;

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
    __super::Late_Update(fTimeDelta);
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fValue", &m_fValue, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iSRVIndex_Fill))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(3)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
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

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    
}
