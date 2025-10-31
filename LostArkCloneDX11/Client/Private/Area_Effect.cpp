#include "pch.h"
#include "Area_Effect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"
#include "Kamen.h"

CArea_Effect::CArea_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CArea_Effect::CArea_Effect(const CArea_Effect& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CArea_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CArea_Effect::Initialize(void* pArg)
{
    CEffect::EFFECT_DESC* pDesc = static_cast<EFFECT_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentTransformCom = pDesc->pParentTransform;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    m_isLoop = true;
    m_iPassIndex = 6;
    m_eType = PARTICLE::CIRCLE;

    m_iNumInstance = 1000;
    m_vSize = _float2(1.5f, 2.5f);
    m_vCenter = _float3(0.f, -55.f, 0.f);
    m_vSpeed = _float2(-1.f, -0.3f);
    m_vRange = _float3(36.f, 1.f, 1.f);
    m_vLifeTime = _float2(2.f, 5.0f);
    m_vPivot = _float3(0.f, -55.f, 0.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_vRotation = _float3(0.f, 0.f, 0.f);

    m_isActive = false;
    m_fTimeAcc = 0.f;

    m_fSizeX = 8.f;
    m_fSizeY = 4.f;

    XMStoreFloat4x4(&m_ParentWorldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    m_isBlur = true;

    m_fRotationSpeed = 1.f;
    m_fRotationAngle = 0.f;


    m_strDiffuseTexture = L"fx_f_shine_004.dds";
    m_strMaskTexture = L"fx_c_cloud_013_cl.dds";

    m_pVIBufferCom->Set_Circle(m_isLoop, m_vCenter, m_vPivot, m_vRange, m_vLifeTime, m_vSpeed, m_vSize);

    return S_OK;
}

void CArea_Effect::Priority_Update(_float fTimeDelta)
{

}

void CArea_Effect::Update(_float fTimeDelta)
{
    m_fRotationAngle += m_fRotationSpeed * fTimeDelta;
    if (m_fRotationAngle >= 360.f)
        m_fRotationAngle = 0.f;
 
    m_pVIBufferCom->Circle(m_vCenter, m_vRange, fTimeDelta);
 
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CArea_Effect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
    m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CArea_Effect::Render()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fSizeX", &m_fSizeX, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fSizeY", &m_fSizeY, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CArea_Effect::Add_Components()
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_AreaEffect"),
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

    /* Shader_VertexPoint */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


CArea_Effect* CArea_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CArea_Effect* pInstance = new CArea_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CArea_Effect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CArea_Effect::Clone(void* pArg)
{
    CGameObject* pInstance = new CArea_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CArea_Effect");
        return nullptr;
    }

    return pInstance;
}

void CArea_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pVIBufferCom);
}
