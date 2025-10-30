#include "pch.h"
#include "Screen_Effect.h"

#include "GameInstance.h"

CScreen_Effect::CScreen_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIPanel{ pDevice, pContext }
{
}

CScreen_Effect::CScreen_Effect(CScreen_Effect& Prototype)
    :CUIPanel{ Prototype }
    , m_isFadeIn{ false }
    , m_isFadeOut{ false }
    , m_fTimeAcc{ 0.f }
    , m_fDuration{ 0.f }
{
}

void CScreen_Effect::Trigger_FadeIn(_float fDuration)
{
    m_isFadeIn = true;
    m_fTimeAcc = 0.f;
    m_fDuration = fDuration;

    m_vScreenColor = _float4(0.f, 0.f, 0.f, 1.f);
}

void CScreen_Effect::Trigger_FadeOut(_float fDuration)
{
    m_isFadeOut = true;
    m_fTimeAcc = 0.f;
    m_fDuration = fDuration;
    m_vScreenColor = _float4(0.f, 0.f, 0.f, 1.f);
}

HRESULT CScreen_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_Effect::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc = {};

    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;
    m_fZ = 0.01f;
    m_fSizeX = (_float)g_iWinSizeX;
    m_fSizeY = (_float)g_iWinSizeY;

    if (FAILED(CGameObject::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    
    _uint i = 1;
    D3D11_VIEWPORT Viewport = {};

    m_pContext->RSGetViewports(&i, &Viewport);
    m_fWinCX = Viewport.Width;
    m_fWinCY = Viewport.Height;

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 1.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));

    m_vScreenColor = _float4(1.f, 1.f, 1.f, 1.f);

    return S_OK;
}

void CScreen_Effect::Priority_Update(_float fTimeDelta)
{
}

void CScreen_Effect::Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_KeyDown(DIK_7))
    {
        Trigger_FadeOut(1.5f);
    }

    if (m_isFadeIn)
        FadeIn(fTimeDelta);
    else if (m_isFadeOut)
        FadeOut(fTimeDelta);
}

void CScreen_Effect::Late_Update(_float fTimeDelta)
{
    if(m_isFadeIn || m_isFadeOut)
        m_pGameInstance->Add_RenderGroup(RENDER::CURSOR, this);
}

HRESULT CScreen_Effect::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vScreenColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(8)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CScreen_Effect::Ready_Components()
{
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

void CScreen_Effect::FadeIn(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    m_vScreenColor.w = m_fTimeAcc / m_fDuration;

    if (m_fDuration < m_fTimeAcc)
    {
        m_isFadeIn = false;
        Trigger_FadeOut(0.5f);
    }
}

void CScreen_Effect::FadeOut(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    m_vScreenColor.w = 1.f - m_fTimeAcc / m_fDuration;

    if (m_fDuration < m_fTimeAcc)
    {
        m_isFadeOut = false;
    }
}

CScreen_Effect* CScreen_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_Effect* pInstance = new CScreen_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CScreen_Effect");
        return nullptr;
    }
    return pInstance;
}

CGameObject* CScreen_Effect::Clone(void* pArg)
{
    CGameObject* pInstance = new CScreen_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CScreen_Effect");
        return nullptr;
    }
    return pInstance;
}

void CScreen_Effect::Free()
{
    __super::Free();
}
