#include "pch.h"
#include "Effect_Particle.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* pArg)
{
    PARTOBJECT_DESC Desc = {};
    Desc.pParentTransform = m_pTransformCom;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    m_Particle_Data.isLoop = true;
    m_Particle_Data.iPassIndex = 6;
    m_Particle_Data.eType = PARTICLE::CIRCLE;
    m_Particle_Data.iNumInstance = 50;
    m_Particle_Data.vSize = _float2(0.05f, 0.1f);
    m_Particle_Data.vCenter = _float3(0.f, 0.f, 0.f);
    m_Particle_Data.vSpeed = _float2(3.f, 5.f);
    m_Particle_Data.vRange = _float3(1.f, 1.f, 1.f);
    m_Particle_Data.vLifeTime = _float2(0.5f, 0.5f);
    m_Particle_Data.vPivot = _float3(0.f, 0.f, 0.f);
    m_Particle_Data.vPosition = _float3(0.f, 0.f, 0.f);
    m_Particle_Data.vRotation = _float3(0.f, 0.f, 0.f);

    m_isActive = false;
    m_fTimeAcc = 0.f;

    m_fSizeX = 1.f;
    m_fSizeY = 1.f;

    XMStoreFloat4x4(&m_ParentWorldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    return S_OK;
}

void CEffect_Particle::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Particle::Update(_float fTimeDelta)
{
    if (true == m_Particle_Data.isLoop)
        m_fTimeAcc += fTimeDelta;

    m_Particle_Data.vLifeTime.x += fTimeDelta;

    switch (m_Particle_Data.eType)
    {
    case Engine::PARTICLE::SPREAD:
        m_pVIBufferCom->Spread(m_Particle_Data.vCenter, m_Particle_Data.vRange, fTimeDelta);
        break;

    case Engine::PARTICLE::SCALE:
        break;

    case Engine::PARTICLE::CIRCLE:
        m_pVIBufferCom->Circle(m_Particle_Data.vCenter, m_Particle_Data.vRange, fTimeDelta);
        break;

    case Engine::PARTICLE::CORN:
        m_pVIBufferCom->Corn(m_Particle_Data.vCenter, m_Particle_Data.vRange, fTimeDelta);
        break;

    default:
        break;
    }

    if (false == m_Particle_Data.isLoop && m_Particle_Data.vLifeTime.y < m_Particle_Data.vLifeTime.x)
        m_isDead = true;
    else if(true == m_Particle_Data.isLoop && m_fTimeAcc > m_Particle_Data.fActiveTime)
        m_isDead = true;

    _matrix matRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Particle_Data.vRotation.x), XMConvertToRadians(m_Particle_Data.vRotation.y), XMConvertToRadians(m_Particle_Data.vRotation.z));

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * matRotation * XMLoadFloat4x4(&m_ParentWorldMatrix));
}

void CEffect_Particle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
    m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CEffect_Particle::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Find_SRV(m_strDiffuseTexture.c_str()))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_MaskTexture", m_pMaskTextureCom->Find_SRV(m_strMaskTexture.c_str()))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pNoiseTextureCom->Find_SRV(m_strNoiseTexture.c_str()))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fSizeX", &m_Particle_Data.vTileSize.x, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fSizeY", &m_Particle_Data.vTileSize.y, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_Particle_Data.iPassIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Particle::Start(const _float4x4* pWorldMatrix, void* pArg)
{
    memcpy(&m_ParentWorldMatrix, pWorldMatrix, sizeof(_float4x4));

    EFFECT_PARTICLE_DESC* pDesc = static_cast<EFFECT_PARTICLE_DESC*>(pArg);

    memcpy(&m_Particle_Data, &pDesc->Data, sizeof(EFFECT_PARTICLE_DATA));

    m_strDiffuseTexture = pDesc->strDiffuseTexture;
    m_strMaskTexture = pDesc->strMaskTexture;
    m_strNoiseTexture = pDesc->strNoiseTexture;

    _matrix matRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Particle_Data.vRotation.x), XMConvertToRadians(m_Particle_Data.vRotation.y), XMConvertToRadians(m_Particle_Data.vRotation.z));

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_Particle_Data.vPosition), 1.f));

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * matRotation * XMLoadFloat4x4(& m_ParentWorldMatrix));

    m_pVIBufferCom->Set_NumInstance(m_Particle_Data.iNumInstance);

    if(PARTICLE::SPREAD == m_Particle_Data.eType)
        m_pVIBufferCom->Set_Spread(m_Particle_Data.isLoop, m_Particle_Data.vCenter, m_Particle_Data.vPivot, m_Particle_Data.vRange, m_Particle_Data.vLifeTime, m_Particle_Data.vSpeed, m_Particle_Data.vSize);
    else if(PARTICLE::CIRCLE == m_Particle_Data.eType)
        m_pVIBufferCom->Set_Circle(m_Particle_Data.isLoop, m_Particle_Data.vCenter, m_Particle_Data.vPivot, m_Particle_Data.vRange, m_Particle_Data.vLifeTime, m_Particle_Data.vSpeed, m_Particle_Data.vSize);
    else if(PARTICLE::CORN == m_Particle_Data.eType)
        m_pVIBufferCom->Set_Corn(m_Particle_Data.isLoop, m_Particle_Data.vCenter, m_Particle_Data.vPivot, m_Particle_Data.vRange, m_Particle_Data.vLifeTime, m_Particle_Data.vSpeed, m_Particle_Data.vSize);

    m_Particle_Data.vLifeTime.x = 0.f;

    m_fTimeAcc = 0.f;

    return S_OK;
}

HRESULT CEffect_Particle::Reset()
{
    m_isDead = false;

    return S_OK;
}

HRESULT CEffect_Particle::Add_Components()
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_TestEffect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
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

    /* Shader_VertexPoint */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_Particle");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_Particle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_Particle");
        return nullptr;
    }

    return pInstance;
}

void CEffect_Particle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);
    Safe_Release(m_pVIBufferCom);
}
