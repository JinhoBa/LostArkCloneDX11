#include "pch.h"
#include "ClashUI.h"

#include "GameInstance.h"

CClashUI::CClashUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIPanel{ pDevice, pContext }
{
}

CClashUI::CClashUI(CClashUI& Prototype)
    :CUIPanel{ Prototype }
{
}

_bool CClashUI::Check_Circle()
{
    if (m_pGameInstance->Get_KeyDown(m_iKey) && 0 == m_iOutCircleTextureIndex)
    {
        if (120.f <= m_fOutSize && 140.f >= m_fOutSize)
        {
            m_iOutCircleTextureIndex = 1;
            return true;
        }
        else
            return false;
    }

    return false;
}

HRESULT CClashUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CClashUI::Initialize(void* pArg)
{
    CLASH_UI_DESC* pDesc = static_cast<CLASH_UI_DESC*>(pArg);

    m_iKey = pDesc->iKey;
    m_fX = pDesc->vPosition.x;
    m_fY = pDesc->vPosition.y;
    m_fZ = 1.f;

    m_fSizeX = m_fSizeY = 130.f;
    m_fOutSize = 200.f;
    m_fKeySize = 42.f;

    m_fTimeAcc = 0.f;
    m_iKeyTextureIndex = (_uint)(m_iKey - DIK_Q);
    m_iOutCircleTextureIndex = 0;

    if (FAILED(CGameObject::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (_float)g_iWinSizeX* 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 1.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));

    m_pOutTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 1.f, 1.f));
    m_pOutTransformCom->Set_Scale(_float3(m_fOutSize, m_fOutSize, 1.f));

    m_pKeyTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 1.f, 1.f));
    m_pKeyTransformCom->Set_Scale(_float3(m_fKeySize, m_fKeySize, 1.f));

    _uint i = 1;
    D3D11_VIEWPORT Viewport = {};

    m_pContext->RSGetViewports(&i, &Viewport);
    m_fWinCX = Viewport.Width;
    m_fWinCY = Viewport.Height;

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

    return S_OK;
}

void CClashUI::Priority_Update(_float fTimeDelta)
{
}

void CClashUI::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if(0 == m_iOutCircleTextureIndex)
    {
        m_fOutSize -= fTimeDelta * 70.f;
    }

    /* fail */
    if (1.5f < m_fTimeAcc)
    {
        m_isDead = true;
    }

    m_pOutTransformCom->Set_Scale(_float3(m_fOutSize, m_fOutSize, 1.f));
}

void CClashUI::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CClashUI::Render()
{
    // UI
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    /* InCircle */
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;

    if (FAILED(Draw_UI()))
        return E_FAIL;

    /* OutCircle */
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pOutTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iOutCircleTextureIndex))))
        return E_FAIL;

    if (FAILED(Draw_UI()))
        return E_FAIL;

    /* Key */
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pKeyTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pKeyTextureCom->Get_SRV(m_iKeyTextureIndex))))
        return E_FAIL;

    if (FAILED(Draw_UI()))
        return E_FAIL;


    return S_OK;
}

HRESULT CClashUI::Ready_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Clash_Circle"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Clash_Key"),
        TEXT("Com_KeyTexture"), reinterpret_cast<CComponent**>(&m_pKeyTextureCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
        TEXT("Com_Shader_VTXPosTex"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*OutTransform*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_OutTransform"), reinterpret_cast<CComponent**>(&m_pOutTransformCom))))
        return E_FAIL;

    /*OutTransform*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_KeyTransform"), reinterpret_cast<CComponent**>(&m_pKeyTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CClashUI::Update_Position()
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 1.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
}

HRESULT CClashUI::Draw_UI()
{
    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

CClashUI* CClashUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CClashUI* pInstance = new CClashUI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CClashUI");
        return nullptr;
    }
    return pInstance;
}

CGameObject* CClashUI::Clone(void* pArg)
{
    CGameObject* pInstance = new CClashUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CClashUI");
        return nullptr;
    }
    return pInstance;
}

void CClashUI::Free()
{
    __super::Free();

    Safe_Release(m_pOutTransformCom);
    Safe_Release(m_pKeyTextureCom);
    Safe_Release(m_pKeyTransformCom);
}
