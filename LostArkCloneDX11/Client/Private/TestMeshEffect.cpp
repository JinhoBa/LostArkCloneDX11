#include "pch.h"
#include "TestMeshEffect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CTestMeshEffect::CTestMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CTestMeshEffect::CTestMeshEffect(const CTestMeshEffect& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CTestMeshEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTestMeshEffect::Initialize(void* pArg)
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
    m_iNumInstance = 300;
    m_vSize = _float3(1.f, 1.f, 1.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPosition = _float3(45.f, 0.f, 45.f);
    m_vRotation = _float3(0.f, 0.f, 0.f);
    m_vDiffuseOffset = _float2(0.f, 0.f);
    m_isActive = false;
    m_fTimeAcc = 0.f;

    XMStoreFloat4x4(&m_CombindedMatrix, XMMatrixIdentity());
    
    m_iMeshIndex = 0;

    auto iter = m_pTextureCom->Get_TextureMap().begin();
    m_strDiffuseTexture = (*iter).first;

    iter = m_pMaskTextureCom->Get_TextureMap().begin();
    m_strMaskTexture = (*iter).first;

    iter = m_pNoiseTextureCom->Get_TextureMap().begin();
    m_strNoiseTexture = (*iter).first;

    fDiffuseScrollSpeedU = 0.f;
    fDiffuseScrollSpeedV = 0.f;

    fMaskScrollSpeedU = 0.f;
    fMaskScrollSpeedV = 0.f;

    return S_OK;
}

void CTestMeshEffect::Priority_Update(_float fTimeDelta)
{

}

void CTestMeshEffect::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Rotation(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));
    m_pTransformCom->Set_Scale(m_vSize);

    m_vDiffuseOffset.x += fDiffuseScrollSpeedU * fTimeDelta;
    m_vDiffuseOffset.y += fDiffuseScrollSpeedV * fTimeDelta;

    m_vMaskOffset.x += fMaskScrollSpeedU * fTimeDelta;
    m_vMaskOffset.y += fMaskScrollSpeedV * fTimeDelta;

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CTestMeshEffect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CTestMeshEffect::Render()
{
#ifdef _DEBUG
    ImGui::InputInt("MehsIndex", (_int*)&m_iMeshIndex);
    if (m_iMeshIndex >= m_EffectModels.size())
        m_iMeshIndex = 0;

    ImGui::SliderFloat("NoiseStrength", (_float*)&m_fNoiseStrength, 0.f, 1.f);

    ImGui::InputFloat("D_OffsetU", (_float*)&fDiffuseScrollSpeedU);
    ImGui::InputFloat("D_OffsetV", (_float*)&fDiffuseScrollSpeedV);
    ImGui::InputFloat("M_OffsetU", (_float*)&fMaskScrollSpeedU);
    ImGui::InputFloat("M_OffsetV", (_float*)&fMaskScrollSpeedV);

    ImGui::InputFloat3("vSize", (_float*)&m_vSize);
    ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
    ImGui::InputFloat3("vPosition", (_float*)&m_vPosition);
    ImGui::SliderFloat3("vRotation", (_float*)&m_vRotation,0.f, 360.f);

    if (ImGui::CollapsingHeader("Diffuse", ImGuiTreeNodeFlags_DefaultOpen))
    {
        _uint iIndex = {};
        for (auto& Pair : m_pTextureCom->Get_TextureMap())
        {
            if (ImGui::ImageButton(to_string(iIndex).c_str(), (ImTextureRef)Pair.second, ImVec2(60.f, 60.f)))
                m_strDiffuseTexture = Pair.first;

            if (4 != iIndex % 5)
                ImGui::SameLine();

            iIndex++;
        }
    }
    if (ImGui::CollapsingHeader("Mask", ImGuiTreeNodeFlags_DefaultOpen))
    {
        _uint iIndex = {};
        for (auto& Pair : m_pMaskTextureCom->Get_TextureMap())
        {
            if (ImGui::ImageButton(to_string(iIndex+100).c_str(), (ImTextureRef)Pair.second, ImVec2(60.f, 60.f)))
                m_strMaskTexture = Pair.first;

            if (4 != iIndex % 5)
                ImGui::SameLine();

            iIndex++;
        }
    }
    if (ImGui::CollapsingHeader("Nosie", ImGuiTreeNodeFlags_DefaultOpen))
    {
        _uint iIndex = {};
        for (auto& Pair : m_pNoiseTextureCom->Get_TextureMap())
        {
            if (ImGui::ImageButton(to_string(iIndex + 200).c_str(), (ImTextureRef)Pair.second, ImVec2(60.f, 60.f)))
                m_strNoiseTexture = Pair.first;

            if (4 != iIndex % 5)
                ImGui::SameLine();

            iIndex++;
        }
    }

#endif // _DEBUG


    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fNoiseStrength", &m_fNoiseStrength, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", &m_pGameInstance->Get_Veiwport().MaxDepth, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(1)))
        return E_FAIL;

    if (FAILED(m_EffectModels[m_iMeshIndex]->Render(0)))
        return S_OK;

    return S_OK;
}

HRESULT CTestMeshEffect::Add_Components()
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
    CModel* pModelCom = { nullptr };

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder1"),
        TEXT("Com_Cylinder1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder2"),
        TEXT("Com_Cylinder2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Dome"),
        TEXT("Com_Dome_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Line"),
        TEXT("Com_Line_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Potal"),
        TEXT("Com_Potal_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Ring"),
        TEXT("Com_Ring_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Screw"),
        TEXT("Com_Screw_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Screwfront"),
        TEXT("Com_Screwfront_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail1"),
        TEXT("Com_Trail1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail2"),
        TEXT("Com_Trail2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail3"),
        TEXT("Com_Trail3_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail4"),
        TEXT("Com_Trail4_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);



    return S_OK;
}


CTestMeshEffect* CTestMeshEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTestMeshEffect* pInstance = new CTestMeshEffect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTestMeshEffect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CTestMeshEffect::Clone(void* pArg)
{
    CGameObject* pInstance = new CTestMeshEffect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTestMeshEffect");
        return nullptr;
    }

    return pInstance;
}

void CTestMeshEffect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);

    for (auto& pModel : m_EffectModels)
        Safe_Release(pModel);
}
