#include "pch.h"
#include "TestMeshEffect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Player.h"

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

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    m_isLoop = true;
    m_isUseScale = false;
    m_isUseRotation = false;
    m_vLifeTime = _float2(1.f, 1.0f);

    m_vStartScale = _float3(1.f, 1.f, 1.f);
    m_vCurrentScale = _float3(1.f, 1.f, 1.f);
    m_vEndScale = _float3(1.f, 1.f, 1.f);

    m_vStartRotation = _float3(0.f, 0.f, 0.f);
    m_vCurRotation = _float3(0.f, 0.f, 0.f);
    m_vEndRotation = _float3(0.f, 0.f, 0.f);

    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_vPivot = _float3(0.f, 0.f, 0.f);

    m_vDiffuseOffset = _float2(0.f, 0.f);

    m_vLifeTime = _float2(0.f, 1.f);
    
    m_iMeshIndex = 0;

    auto iter = m_pTextureCom->Get_TextureMap().begin();
    m_strDiffuseTexture = (*iter).first;

    iter = m_pMaskTextureCom->Get_TextureMap().begin();
    m_strMaskTexture = (*iter).first;

    iter = m_pNoiseTextureCom->Get_TextureMap().begin();
    m_strNoiseTexture = (*iter).first;

    m_fDiffuseScrollSpeedU = 0.f;
    m_fDiffuseScrollSpeedV = 0.f;
    
    m_fMaskScrollSpeedU = 0.f;
    m_fMaskScrollSpeedV = 0.f;

    m_fSpeed = 0.f;
    m_fDissolveSpeed = 1.f;

    XMStoreFloat4x4(&m_ParentWorldMatrix, XMMatrixIdentity());

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Set_Scale(m_vStartScale);

    return S_OK;
}

void CTestMeshEffect::Priority_Update(_float fTimeDelta)
{
    
}

void CTestMeshEffect::Update(_float fTimeDelta)
{
    /* LifeTime */
    m_vLifeTime.x += fTimeDelta;

    /* Scale */
    if (m_isUseScale)
    {
        XMStoreFloat3(&m_vCurrentScale, XMVectorLerp(XMLoadFloat3(&m_vStartScale), XMLoadFloat3(&m_vEndScale), m_vLifeTime.x / m_vLifeTime.y));
        m_pTransformCom->Set_Scale(m_vCurrentScale);
    }
    else
        m_pTransformCom->Set_Scale(m_vStartScale);

    /* Rotation */
    if(m_isUseRotation)
    {
        XMStoreFloat3(&m_vCurRotation, XMQuaternionSlerp(XMLoadFloat3(&m_vStartRotation), XMLoadFloat3(&m_vEndRotation), m_vLifeTime.x / m_vLifeTime.y));
        m_pTransformCom->Rotation(XMConvertToRadians(m_vCurRotation.x), XMConvertToRadians(m_vCurRotation.y), XMConvertToRadians(m_vCurRotation.z));
    }
    else
    {
        m_pTransformCom->Rotation(XMConvertToRadians(m_vStartRotation.x), XMConvertToRadians(m_vStartRotation.y), XMConvertToRadians(m_vStartRotation.z));
    }

    /* Position */
    m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_Position() + XMVectorSetW(XMLoadFloat3(&m_vPivot), 0.f) * fTimeDelta * m_fSpeed);


    /* Texture UV */
    if (0.f == m_fDiffuseScrollSpeedU)
        m_vDiffuseOffset.x = 0.f;
    else
        m_vDiffuseOffset.x += m_fDiffuseScrollSpeedU * fTimeDelta;
    
    if (0.f == m_fDiffuseScrollSpeedV)
        m_vDiffuseOffset.y = 0.f;
    else
        m_vDiffuseOffset.y += m_fDiffuseScrollSpeedV * fTimeDelta;

    if (0.f == m_fMaskScrollSpeedU)
        m_vMaskOffset.x = 0.f;
    else
        m_vMaskOffset.x += m_fMaskScrollSpeedU * fTimeDelta;

    if (0.f == m_fMaskScrollSpeedV)
        m_vMaskOffset.y = 0.f;
    else
        m_vMaskOffset.y += m_fMaskScrollSpeedV * fTimeDelta;


    /* Reset */
    if (true == m_isLoop && m_vLifeTime.y < m_vLifeTime.x)
    {
        m_vLifeTime.x = 0.f;
        m_pTransformCom->Set_Scale(m_vStartScale);
        m_pTransformCom->Rotation(XMConvertToRadians(m_vStartRotation.x), XMConvertToRadians(m_vStartRotation.y), XMConvertToRadians(m_vStartRotation.z));
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));

        m_vDiffuseOffset.x = m_vDiffuseOffset.y = 0.f;
        m_vMaskOffset.x = m_vMaskOffset.y = 0.f;

        m_ParentWorldMatrix = dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(
            ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Get_Transform()->Get_WorldMatrix();
    }


    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_ParentWorldMatrix));
}

void CTestMeshEffect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CTestMeshEffect::Render()
{
#ifdef _DEBUG
    ImGui::SliderFloat("PerTestDeltaTime", &g_fTestDeltaTime, 0.f, 1.f);
    ImGui::InputInt("ID", (_int*)&m_iEffectID);
    ImGui::Spacing();

    ImGui::InputInt("MeshIndex", (_int*)&m_iMeshIndex);
    if (m_iMeshIndex >= m_EffectModels.size())
        m_iMeshIndex = 0;

    ImGui::Checkbox("Loop", &m_isLoop);
    ImGui::InputFloat("LifeTime", (_float*)&m_vLifeTime.y);
    ImGui::Spacing();
    
    if (ImGui::CollapsingHeader("Scale", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Use_Scale", &m_isUseScale);
        ImGui::InputFloat3("StartScale", (_float*)&m_vStartScale);
        ImGui::InputFloat3("EndScale", (_float*)&m_vEndScale);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Rotation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Use_Rotation", &m_isUseRotation);
        ImGui::SliderFloat3("Start_Rotation", (_float*)&m_vStartRotation, 0.f, 360.f);
        ImGui::SliderFloat3("End_Rotation", (_float*)&m_vEndRotation, 0.f, 360.f);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("vPosition", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputFloat3("vPosition", (_float*)&m_vPosition);
        ImGui::InputFloat3("vPivot", (_float*)&m_vPivot);
        ImGui::InputFloat("fSpeed", (_float*)&m_fSpeed);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Shader"))
    {
        ImGui::SliderFloat("NoiseStrength", (_float*)&m_fNoiseStrength, 0.f, 1.f);
        ImGui::InputFloat("D_OffsetU", (_float*)&m_fDiffuseScrollSpeedU);
        ImGui::InputFloat("D_OffsetV", (_float*)&m_fDiffuseScrollSpeedV);
        ImGui::InputFloat("M_OffsetU", (_float*)&m_fMaskScrollSpeedU);
        ImGui::InputFloat("M_OffsetV", (_float*)&m_fMaskScrollSpeedV);
        ImGui::SliderFloat("Dissolve", (_float*)&m_fDissolveSpeed,0.f, 1.f);
    }
    ImGui::Spacing();

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
    ImGui::Spacing();
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
    ImGui::Spacing();
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

    if (ImGui::Button("Save"))
    {
        EFFECT_MESH_DESC Desc = {};
        EFFECT_MESH_DATA Data = {};

        Data.isLoop = m_isLoop;
        Data.isUseScale = m_isUseScale;
        Data.isUseRotation = m_isUseRotation;

        Data.iMeshIndex = m_iMeshIndex;
        Data.vPosition = m_vPosition;
        Data.vPivot = m_vPivot;

        Data.vStartRotation = m_vStartRotation;
        Data.vEndRotation = m_vEndRotation;

        Data.vStartScale = m_vStartScale;
        Data.vEndScale = m_vEndScale;

        Data.vLifeTime = _float2(0.f, m_vLifeTime.y);
        Data.fNoiseStrength = m_fNoiseStrength;
        Data.fSpeed = m_fSpeed;

        Data.fDiffuseScrollSpeedU = m_fDiffuseScrollSpeedU;
        Data.fDiffuseScrollSpeedV = m_fDiffuseScrollSpeedV;

        Data.fMaskScrollSpeedU = m_fMaskScrollSpeedU;
        Data.fMaskScrollSpeedV = m_fMaskScrollSpeedV;

        Desc.Mesh_Data = Data;
        Desc.strDiffuseTexture = m_strDiffuseTexture;
        Desc.strMaskTexture = m_strMaskTexture;
        Desc.strNoiseTexture = m_strNoiseTexture;

        if(FAILED(CGameManager::GetInstance()->Save_Effect(EFFECT::MESH, &Desc, "../Bin/Resources/Data/Effect/Player_Effects.xml", m_iEffectID)))
            MSG_BOX("저장 실패");
    }

#endif // _DEBUG

    if (FAILED(Bind_ShaderResource()))
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

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail5"),
        TEXT("Com_Trail5_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Auro"),
        TEXT("Com_Auro_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_1"),
        TEXT("Com_Decal_1_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_2"),
        TEXT("Com_Decal_2_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_3"),
        TEXT("Com_Decal_3_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);
    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder3"),
        TEXT("Com_Cylinder3_Model"), reinterpret_cast<CComponent**>(&pModelCom))))
        return E_FAIL;
    m_EffectModels.push_back(pModelCom);



    return S_OK;
}

HRESULT CTestMeshEffect::Bind_ShaderResource()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveSpeed", &m_fDissolveSpeed, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLifeTime", &m_vLifeTime, sizeof(_float2))))
        return E_FAIL;

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
