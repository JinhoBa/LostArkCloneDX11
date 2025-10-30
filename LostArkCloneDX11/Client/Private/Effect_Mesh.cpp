#include "pch.h"
#include "Effect_Mesh.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Player.h"

CEffect_Mesh::CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CEffect_Mesh::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void* pArg)
{
    PARTOBJECT_DESC Desc = {};
    Desc.pParentTransform = m_pTransformCom;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    m_Effect_Data.isLoop = false;
    m_Effect_Data.isUseScale = false;
    m_Effect_Data.isUseRotation = false;
    m_Effect_Data.vLifeTime = _float2(1.f, 1.0f);

    m_Effect_Data.vStartScale = _float3(1.f, 1.f, 1.f);
    m_vCurrentScale = _float3(1.f, 1.f, 1.f);
    m_Effect_Data.vEndScale = _float3(1.f, 1.f, 1.f);

    m_Effect_Data.vStartRotation = _float3(0.f, 0.f, 0.f);
    m_vCurRotation = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vEndRotation = _float3(0.f, 0.f, 0.f);

    m_Effect_Data.vPosition = _float3(0.f, 0.f, 0.f);
    m_Effect_Data.vPivot = _float3(0.f, 0.f, 0.f);

    m_vDiffuseOffset = _float2(0.f, 0.f);

    m_Effect_Data.iMeshIndex = 0;

    auto iter = m_pTextureCom->Get_TextureMap().begin();
    m_strDiffuseTexture = (*iter).first;

    iter = m_pMaskTextureCom->Get_TextureMap().begin();
    m_strMaskTexture = (*iter).first;

    iter = m_pNoiseTextureCom->Get_TextureMap().begin();
    m_strNoiseTexture = (*iter).first;

    m_Effect_Data.fDiffuseScrollSpeedU = 0.f;
    m_Effect_Data.fDiffuseScrollSpeedV = 0.f;

    m_Effect_Data.fMaskScrollSpeedU = 0.f;
    m_Effect_Data.fMaskScrollSpeedV = 0.f;

    m_Effect_Data.fSpeed = 0.f;
    m_fDissolveSpeed = 1.f;

    XMStoreFloat4x4(&m_ParentWorldMatrix, XMMatrixIdentity());

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));

    return S_OK;
}

void CEffect_Mesh::Priority_Update(_float fTimeDelta)
{

}

void CEffect_Mesh::Update(_float fTimeDelta)
{
    /* LifeTime */
    m_Effect_Data.vLifeTime.x += fTimeDelta;

    /* Scale */
    if (m_Effect_Data.isUseScale)
    {
        XMStoreFloat3(&m_vCurrentScale, XMVectorLerp(
            XMLoadFloat3(&m_Effect_Data.vStartScale),
            XMLoadFloat3(&m_Effect_Data.vEndScale), 
            m_Effect_Data.vLifeTime.x / m_Effect_Data.vLifeTime.y));
        m_pTransformCom->Set_Scale(m_vCurrentScale);
    }
    else
        m_pTransformCom->Set_Scale(m_Effect_Data.vStartScale);

    /* Rotation */
    if (m_Effect_Data.isUseRotation)
    {
        XMStoreFloat3(&m_vCurRotation, XMQuaternionSlerp(
            XMLoadFloat3(&m_Effect_Data.vStartRotation), XMLoadFloat3(&m_Effect_Data.vEndRotation), m_Effect_Data.vLifeTime.x / m_Effect_Data.vLifeTime.y));
        m_pTransformCom->Rotation(XMConvertToRadians(m_vCurRotation.x), XMConvertToRadians(m_vCurRotation.y), XMConvertToRadians(m_vCurRotation.z));
    }
    else
    {
        m_pTransformCom->Rotation(XMConvertToRadians(m_Effect_Data.vStartRotation.x), XMConvertToRadians(m_Effect_Data.vStartRotation.y), XMConvertToRadians(m_Effect_Data.vStartRotation.z));
    }

    /* Position */
    m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_Position() + XMVectorSetW(XMLoadFloat3(&m_Effect_Data.vPivot), 0.f) * fTimeDelta * m_Effect_Data.fSpeed);

    /* Texture UV */
    if (0.f == m_Effect_Data.fDiffuseScrollSpeedU)
        m_vDiffuseOffset.x = 0.f;
    else
        m_vDiffuseOffset.x += m_Effect_Data.fDiffuseScrollSpeedU * fTimeDelta;

    if (0.f == m_Effect_Data.fDiffuseScrollSpeedV)
        m_vDiffuseOffset.y = 0.f;
    else
        m_vDiffuseOffset.y += m_Effect_Data.fDiffuseScrollSpeedV * fTimeDelta;

    if (0.f == m_Effect_Data.fMaskScrollSpeedU)
        m_vMaskOffset.x = 0.f;
    else
        m_vMaskOffset.x += m_Effect_Data.fMaskScrollSpeedU * fTimeDelta;

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

        m_ParentWorldMatrix = dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(
            ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Get_Transform()->Get_WorldMatrix();
    }
    else if (false == m_Effect_Data.isLoop && m_Effect_Data.vLifeTime.y < m_Effect_Data.vLifeTime.x)
        m_isDead = true;

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_ParentWorldMatrix));
}

void CEffect_Mesh::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect_Mesh::Render()
{
    if (FAILED(Bind_ShaderResource()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_Effect_Data.iPassIndex)))
        return E_FAIL;

    if (FAILED(m_EffectModels[m_Effect_Data.iMeshIndex]->Render(0)))
        return S_OK;

    return S_OK;
}

HRESULT CEffect_Mesh::Start(const _float4x4* pWorldMatrix, void* pArg)
{
    memcpy(&m_ParentWorldMatrix, pWorldMatrix, sizeof(_float4x4));

    EFFECT_MESH_DESC* pDesc = static_cast<EFFECT_MESH_DESC*>(pArg);

    memcpy(&m_Effect_Data, &pDesc->Mesh_Data, sizeof(EFFECT_MESH_DATA));

    m_strDiffuseTexture = pDesc->strDiffuseTexture;
    m_strMaskTexture = pDesc->strMaskTexture;
    m_strNoiseTexture = pDesc->strNoiseTexture;

    m_pTransformCom->Set_Scale(m_Effect_Data.vStartScale);
    m_pTransformCom->Rotation(XMConvertToRadians(m_Effect_Data.vStartRotation.x), XMConvertToRadians(m_Effect_Data.vStartRotation.y), XMConvertToRadians(m_Effect_Data.vStartRotation.z));
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_Effect_Data.vPosition), 1.f));

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_ParentWorldMatrix));

    return S_OK;
}

HRESULT CEffect_Mesh::Reset()
{
    m_isDead = false;

    return S_OK;
}

HRESULT CEffect_Mesh::Add_Components()
{
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

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMeshEffect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

#pragma region EFFECT_MODEL
    CModel* pModelCom = { nullptr };

    /* 0 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder1"),
        TEXT("Com_Cylinder1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 1 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Ring"),
        TEXT("Com_Ring_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 2 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Screwfront"),
        TEXT("Com_Screwfront_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 3 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail1"),
        TEXT("Com_Trail1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* 4 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail2"),
        TEXT("Com_Trail2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 5 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail3"),
        TEXT("Com_Trail3_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 6 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail4"),
        TEXT("Com_Trail4_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 7 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Auro"),
        TEXT("Com_Auro_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 8 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_3"),
        TEXT("Com_Decal_3_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 9 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Picking"),
        TEXT("Com_Picking_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 10 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Hron"),
        TEXT("Com_Horn_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 11 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane1"),
        TEXT("Com_Plane1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 12 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plan1"),
        TEXT("Com_Plan1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 13 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plan2"),
        TEXT("Com_Plan2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 14 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Halfsphere1"),
        TEXT("Com_Halfsphere1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 15 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Halfsphere2"),
        TEXT("Com_Halfsphere2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 16 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Line2"),
        TEXT("Com_Line2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 17 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Planecrossup"),
        TEXT("Com_Planecrossup_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 18 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Bendplane1"),
        TEXT("Com_Bendpane1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* 19 */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_KamenBack"),
        TEXT("Com_KamenBack"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
#pragma endregion

    return S_OK;
}

HRESULT CEffect_Mesh::Bind_ShaderResource()
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


CEffect_Mesh* CEffect_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Mesh* pInstance = new CEffect_Mesh(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_Mesh");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_Mesh::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_Mesh(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_Mesh");
        return nullptr;
    }

    return pInstance;
}

void CEffect_Mesh::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);

    for (auto& pModel : m_EffectModels)
        Safe_Release(pModel);
}
