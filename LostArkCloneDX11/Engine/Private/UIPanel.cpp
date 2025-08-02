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
}

HRESULT CUIPanel::Render()
{
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
