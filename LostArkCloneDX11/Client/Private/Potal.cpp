#include "pch.h"
#include "Potal.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Player.h"
#include "Level_Loading.h"

CPotal::CPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CPotal::CPotal(const CPotal& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CPotal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPotal::Initialize(void* pArg)
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

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(40.f, 2.3f, 22.2f, 1.f));
    m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (nullptr == m_pPlayerTransformCom)
        return E_FAIL;

    Safe_AddRef(m_pPlayerTransformCom);

    m_isEnter = false;

    return S_OK;
}

void CPotal::Priority_Update(_float fTimeDelta)
{
    if(!m_isDead)
        Check_Enter();
}

void CPotal::Update(_float fTimeDelta)
{
    /* Check Player Position */
    
}

void CPotal::Late_Update(_float fTimeDelta)
{
    if (!m_isDead)
    {
        m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
        m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
    }

}

HRESULT CPotal::Render()
{
    if (FAILED(Bind_ShaderResource()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pModelCom->Render(0)))
        return S_OK;

    return S_OK;
}

HRESULT CPotal::Add_Components()
{
    CGameManager::GetInstance()->Bind_Effect_Resource(&m_pTextureCom, &m_pMaskTextureCom, &m_pNoiseTextureCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMeshEffect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Potal"),
        TEXT("Com_Trail1_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPotal::Bind_ShaderResource()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Find_SRV(L"fx_e_atypical_031.dds"))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_MaskTexture", m_pMaskTextureCom->Find_SRV(L"fx_b_glow_001_cl.dds"))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pNoiseTextureCom->Find_SRV(L"fx_b_noise_001.dds"))))
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

void CPotal::Check_Enter()
{
    if (2.5f >= XMVectorGetX(XMVector3Length(m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position())))
    {
        m_isEnter = true;

        CGameManager::GetInstance()->Trigger_Interaction(true, 0, m_pTransformCom->Get_Position());

        if (m_pGameInstance->Get_KeyDown(DIK_F1))
        {
            CGameManager::GetInstance()->Trigger_Interaction(false, 0, m_pTransformCom->Get_Position());
            m_isDead = true;
        }
    }
    else
    {
        CGameManager::GetInstance()->Trigger_Interaction(false, 0, m_pTransformCom->Get_Position());
    }
}

CPotal* CPotal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPotal* pInstance = new CPotal(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CPotal");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CPotal::Clone(void* pArg)
{
    CGameObject* pInstance = new CPotal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CPotal");
        return nullptr;
    }

    return pInstance;
}

void CPotal::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerTransformCom);

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);

    Safe_Release(m_pModelCom);
}
