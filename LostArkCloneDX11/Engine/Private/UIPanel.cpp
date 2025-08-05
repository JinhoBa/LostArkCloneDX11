#include "UIPanel.h"

#include "GameInstance.h"

CUIPanel::CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{pDevice, pContext}
{
}

CUIPanel::CUIPanel(const CUIPanel& Prototype)
    : CUIObject{Prototype}
{
}

HRESULT CUIPanel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIPanel::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUIPanel::Priority_Update(_float fTimeDelta)
{
   
}

void CUIPanel::Update(_float fTimeDelta)
{
}

void CUIPanel::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUIPanel::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUIPanel::Bind_ShaderResource(_uint iSVRIndex)
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(iSVRIndex))))
        return E_FAIL;

    return S_OK;
}

CUIPanel* CUIPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIPanel* pInstance = new CUIPanel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CUIPanel");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CUIPanel::Clone(void* pArg)
{
    CGameObject* pInstance = new CUIPanel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CUIPanel");
        return nullptr;
    }

    return pInstance;
}

void CUIPanel::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
}
