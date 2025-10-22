#include "pch.h"
#include "Effect_Ground.h"

#include "GameInstance.h"

CEffect_Ground::CEffect_Ground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CEffect_Ground::CEffect_Ground(const CEffect_Ground& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CEffect_Ground::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Ground::Initialize(void* pArg)
{
    PARTOBJECT_DESC Desc = {};

    Desc.fRotatePersec = 1.f;
    Desc.fSpeedPersec = 1.f;
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.1f, 0.f, 1.f));

    m_Effect_Data.eLerpType = LERP::LINEAR;
    m_Effect_Data.isRotation = false;
    m_Effect_Data.iID = 0;
    m_Effect_Data.iPassIndex = 4;
    m_Effect_Data.vSize = _float2(0.1f, 2.f);
    m_Effect_Data.vLifeTime = _float2(0.f, 1.0f);
    m_Effect_Data.vPivot = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vPosition = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vStartRotation = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vEndRotation = _float3(0.f, 0.f, 0.f);

    auto iter = m_pTextureCom->Get_TextureMap().begin();
    m_strDiffuseTexture = (*iter).first;

    iter = m_pMaskTextureCom->Get_TextureMap().begin();
    m_strMaskTexture = (*iter).first;

    iter = m_pNoiseTextureCom->Get_TextureMap().begin();
    m_strNoiseTexture = (*iter).first;

    return S_OK;
}

void CEffect_Ground::Priority_Update(_float fTimeDelta)
{

}

void CEffect_Ground::Update(_float fTimeDelta)
{
    m_Effect_Data.vLifeTime.x += fTimeDelta;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_Effect_Data.vPosition), 1.f));

    if (m_Effect_Data.isRotation)
    {
        _vector vCurRotaiton = XMVectorLerp(XMLoadFloat3(&m_Effect_Data.vStartRotation),
            XMLoadFloat3(&m_Effect_Data.vEndRotation),
            m_Effect_Data.vLifeTime.x / m_Effect_Data.vLifeTime.y);

        m_pTransformCom->Rotation(
            XMConvertToRadians(vCurRotaiton.m128_f32[0]),
            XMConvertToRadians(vCurRotaiton.m128_f32[1]),
            XMConvertToRadians(vCurRotaiton.m128_f32[2]));
    }

    m_pVIBufferCom->Scaling(fTimeDelta, m_Effect_Data.eLerpType, m_Effect_Data.vPivot, m_Effect_Data.fSpeed);

    if (true == m_Effect_Data.vLifeTime.y < m_Effect_Data.vLifeTime.x)
        m_isDead = true;

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())
        * XMLoadFloat4x4(&m_ParentWorldMatrix));
}

void CEffect_Ground::Late_Update(_float fTimeDelta)
{
   m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
   m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect_Ground::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Find_SRV(m_strDiffuseTexture.c_str()))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_MaskTexture", m_pMaskTextureCom->Find_SRV(m_strMaskTexture.c_str()))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pNoiseTextureCom->Find_SRV(m_strNoiseTexture.c_str()))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_Effect_Data.iPassIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Ground::Start(const _float4x4* pWorldMatrix, void* pArg)
{
    memcpy(&m_ParentWorldMatrix, pWorldMatrix, sizeof(_float4x4));

    EFFECT_GROUND_DESC* pDesc = static_cast<EFFECT_GROUND_DESC*>(pArg);

    memcpy(&m_Effect_Data, &(pDesc->Ground_Data), sizeof(EFFECT_GROUND_DATA));

    m_strDiffuseTexture = pDesc->strDiffuseTexture;
    m_strMaskTexture = pDesc->strMaskTexture;
    m_strNoiseTexture = pDesc->strNoiseTexture;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_Effect_Data.vPosition), 1.f));
    m_pTransformCom->Rotation(XMConvertToRadians(m_Effect_Data.vStartRotation.x), XMConvertToRadians(m_Effect_Data.vStartRotation.y), XMConvertToRadians(m_Effect_Data.vStartRotation.z));

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_ParentWorldMatrix));

    m_pVIBufferCom->Set_Desc(false, m_Effect_Data.vSize, m_Effect_Data.vLifeTime);
    m_pVIBufferCom->Scaling(0.f, m_Effect_Data.eLerpType, m_Effect_Data.vPivot, m_Effect_Data.fSpeed);

    m_pVIBufferCom->Reset();

    return S_OK;
}

HRESULT CEffect_Ground::Reset()
{
    m_isDead = false;

    return S_OK;
}

void CEffect_Ground::Add_EffectComponets(CTexture* pTextureCom, CTexture* pMaskTextureCom, CTexture* pNoiseTextureCom)
{
    m_pTextureCom = pTextureCom;
    m_pMaskTextureCom = pMaskTextureCom;
    m_pNoiseTextureCom = pNoiseTextureCom;

    Safe_AddRef(m_pTextureCom);
    Safe_AddRef(m_pMaskTextureCom);
    Safe_AddRef(m_pNoiseTextureCom);

    auto iter = m_pTextureCom->Get_TextureMap().begin();
    m_strDiffuseTexture = (*iter).first;

    iter = m_pMaskTextureCom->Get_TextureMap().begin();
    m_strMaskTexture = (*iter).first;

    iter = m_pNoiseTextureCom->Get_TextureMap().begin();
    m_strNoiseTexture = (*iter).first;
}

HRESULT CEffect_Ground::Add_Components(void* pArg)
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_GroundEffect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_DiffuseFolder"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_MaskFolder"),
        TEXT("Com_MaskTexture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_NoiseFolder"),
        TEXT("Com_NoiseTexture"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    return S_OK;
}

CEffect_Ground* CEffect_Ground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Ground* pInstance = new CEffect_Ground(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_Ground");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_Ground::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_Ground(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_Ground");
        return nullptr;
    }

    return pInstance;
}

void CEffect_Ground::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);
}
