#include "pch.h"
#include "Effect_BossEnter.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Player.h"

CEffect_BossEnter::CEffect_BossEnter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CEffect_BossEnter::CEffect_BossEnter(const CEffect_BossEnter& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CEffect_BossEnter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_BossEnter::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_Effect_Data.isLoop = true;
    m_Effect_Data.isUseScale = false;
    m_Effect_Data.isUseRotation = false;
    m_Effect_Data.vLifeTime = _float2(0.f, 2.0f);

    m_Effect_Data.vStartScale = _float3(5.f, 5.f, 5.f);
    m_vCurrentScale = _float3(5.f, 5.f, 5.f);
    m_Effect_Data.vEndScale = _float3(5.5f, 5.5f, 5.5f);

    m_Effect_Data.vStartRotation = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vEndRotation = _float3(0.f, 0.f, 0.f);

    m_Effect_Data.vPosition = _float3(37.f, 14.1f, 9.f);
    m_Effect_Data.vPivot = _float3(0.f, 0.f, 0.f);

    m_vDiffuseOffset = _float2(0.f, 0.f);

    m_Effect_Data.iMeshIndex = 0;

    m_Effect_Data.fDiffuseScrollSpeedU = 0.f;
    m_Effect_Data.fDiffuseScrollSpeedV = 0.f;

    m_Effect_Data.fMaskScrollSpeedU = 0.f;
    m_Effect_Data.fMaskScrollSpeedV = 0.9f;

    m_Effect_Data.fSpeed = 0.f;
    m_fDissolveSpeed = 0.f;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(37.f, 14.1f, 9.f, 1.f));
    m_pTransformCom->Set_Scale(m_Effect_Data.vStartScale);

    m_strDiffuseTexture = L"fx_l_color_blue_01.dds";

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (nullptr == m_pPlayerTransformCom)
        return E_FAIL;

    Safe_AddRef(m_pPlayerTransformCom);

    m_isEnter = false;

    return S_OK;
}

void CEffect_BossEnter::Priority_Update(_float fTimeDelta)
{

}

void CEffect_BossEnter::Update(_float fTimeDelta)
{
    /* LifeTime */
    m_Effect_Data.vLifeTime.x += fTimeDelta;

    /* Texture UV */
    if (0.f == m_Effect_Data.fMaskScrollSpeedV)
        m_vMaskOffset.y = 0.f;
    else
        m_vMaskOffset.y += m_Effect_Data.fMaskScrollSpeedV * fTimeDelta;

    /* Reset */
    if (true == m_Effect_Data.isLoop && m_Effect_Data.vLifeTime.y < m_Effect_Data.vLifeTime.x)
    {
        m_Effect_Data.vLifeTime.x = 0.f;
        m_pTransformCom->Set_Scale(m_Effect_Data.vStartScale);
        m_pTransformCom->Rotation(XMConvertToRadians(m_Effect_Data.vStartRotation.x), XMConvertToRadians(m_Effect_Data.vStartRotation.y), XMConvertToRadians(m_Effect_Data.vStartRotation.z));
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_Effect_Data.vPosition), 1.f));

        m_vDiffuseOffset.x = m_vDiffuseOffset.y = 0.f;
        m_vMaskOffset.x = m_vMaskOffset.y = 0.f;

     
    }
    else if (false == m_Effect_Data.isLoop && m_Effect_Data.vLifeTime.y < m_Effect_Data.vLifeTime.x)
        m_isDead = true;

    /* Check Player Position */
    Check_Enter();
}

void CEffect_BossEnter::Late_Update(_float fTimeDelta)
{
    if(!m_isDead)
    {
        m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
        m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
    }
}

HRESULT CEffect_BossEnter::Render()
{
    if (FAILED(Bind_ShaderResource()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pModelCom->Render(0)))
        return S_OK;

    return S_OK;
}

HRESULT CEffect_BossEnter::Add_Components()
{
    CGameManager::GetInstance()->Bind_Effect_Resource(&m_pTextureCom, &m_pMaskTextureCom, &m_pNoiseTextureCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMeshEffect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Ring"),
        TEXT("Com_Trail1_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_BossEnter::Bind_ShaderResource()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Find_SRV(m_strDiffuseTexture.c_str()))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_MaskTexture", m_pMaskTextureCom->Find_SRV(L"fx_a_line_006_ycl.dds"))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pNoiseTextureCom->Find_SRV(L"fx_c_noise_004.dds"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vDiffuseOffset", &m_vDiffuseOffset, sizeof(_float2))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vMaskOffset", &m_vMaskOffset, sizeof(_float2))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveSpeed", &m_fDissolveSpeed, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLifeTime", &m_Effect_Data.vLifeTime, sizeof(_float2))))
        return E_FAIL;

    return S_OK;
}

void CEffect_BossEnter::Check_Enter()
{
    if (2.5f >= XMVectorGetX(XMVector3Length(m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position())))
    {
        m_isEnter = true;
        m_strDiffuseTexture = L"fx_l_color_blue_01.dds";
        m_vLightColor = _float4(0.f, 0.f, 1.f, 1.f);
        m_pGameInstance->Update_Light_Color(L"BossEnter_Light", 0, &m_vLightColor);
        m_pGameInstance->Update_Light_Color(L"BossEnter_Light", 1, &m_vLightColor);
    }
    else
    {
        m_isEnter = false;
        m_strDiffuseTexture = L"fx_l_color_green_01.dds";
        m_vLightColor = _float4(0.f, 1.f, 0.f, 1.f);
        m_pGameInstance->Update_Light_Color(L"BossEnter_Light", 0, &m_vLightColor);
        m_pGameInstance->Update_Light_Color(L"BossEnter_Light", 1, &m_vLightColor);
    }
}


CEffect_BossEnter* CEffect_BossEnter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_BossEnter* pInstance = new CEffect_BossEnter(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_BossEnter");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_BossEnter::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_BossEnter(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_BossEnter");
        return nullptr;
    }

    return pInstance;
}

void CEffect_BossEnter::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerTransformCom);

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);

    Safe_Release(m_pModelCom);
}
