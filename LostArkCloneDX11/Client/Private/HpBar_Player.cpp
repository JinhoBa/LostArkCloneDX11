#include "pch.h"
#include "HpBar_Player.h"

#include "GameInstance.h"

CHpBar_Player::CHpBar_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CHpBar_Player::CHpBar_Player(const CHpBar_Player& Prototype)
    :CPartObject{ Prototype }
{
}

HRESULT CHpBar_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHpBar_Player::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    HPBARPLAYER_DESC* pDesc = static_cast<HPBARPLAYER_DESC*>(pArg);

    m_pPlayerInfo = pDesc->pPlayerInfo;
    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Ready_Font()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 2.f, 0.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(0.6f, 0.07f, 1.f));


    return S_OK;
}

void CHpBar_Player::Priority_Update(_float fTimeDelta)
{

}

void CHpBar_Player::Update(_float fTimeDelta)
{
    m_fValue = m_pPlayerInfo->fHp / m_pPlayerInfo->fMaxHp;

    _vector vPosition;

    _matrix CombinedWorldMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix());


    memcpy(&vPosition, &CombinedWorldMatrix.r[3], sizeof(_float4));

    m_pTransformCom->Set_State(STATE::POSITION, vPosition);
    m_pTransformCom->BillBoard(XMLoadFloat4(m_pGameInstance->Get_Camera_Look()));

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));


    vPosition = XMVector3TransformCoord(vPosition, XMLoadFloat4x4(m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW)));

    vPosition = XMVector3TransformCoord(vPosition, XMLoadFloat4x4(m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ)));

    m_PlayerNameFont.vPositon.x = (_float)g_iWinSizeX * 0.5f + vPosition.m128_f32[0] * (_float)g_iWinSizeX * 0.5f - 25.f;
    m_PlayerNameFont.vPositon.y = (_float)g_iWinSizeY * 0.5f - vPosition.m128_f32[1] * (_float)g_iWinSizeY * 0.5f - 20.f;
}

void CHpBar_Player::Late_Update(_float fTimeDelta)
{
   

    m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &m_PlayerNameFont);
    m_pGameInstance->Add_RenderGroup(RENDER::WORLDUI, this);
}

HRESULT CHpBar_Player::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    /* Back */
    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    /* Gauge */
    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(1))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fValue", &m_fValue, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(3)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CHpBar_Player::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_WorldHpBar"),
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
HRESULT CHpBar_Player::Ready_Font()
{
    m_PlayerNameFont.strWord = L"  NULL";
    m_PlayerNameFont.vPositon = _float2(0.f, 0.f);
    m_PlayerNameFont.vColor = _float4(0.9f, 0.9f, 0.7f, 1.f);
    m_PlayerNameFont.fScale = 0.36f;

    return S_OK;
}

CHpBar_Player* CHpBar_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHpBar_Player* pInstance = new CHpBar_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CHpBar_Player");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CHpBar_Player::Clone(void* pArg)
{
    CGameObject* pInstance = new CHpBar_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CHpBar_Player");
        return nullptr;
    }

    return pInstance;
}

void CHpBar_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
