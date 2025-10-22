#include "pch.h"
#include "Test_Ground_Effect.h"

#include "GameInstance.h"
#include "GameManager.h"

CTest_Ground_Effect::CTest_Ground_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CTest_Ground_Effect::CTest_Ground_Effect(const CTest_Ground_Effect& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CTest_Ground_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTest_Ground_Effect::Initialize(void* pArg)
{
    PARTOBJECT_DESC Desc = {};

    Desc.fRotatePersec = 1.f;
    Desc.fSpeedPersec = 1.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.1f, 0.f, 1.f));

    m_vSize = _float2(0.1f, 2.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPivot = _float3(0.f, 0.f, 0.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_isRotation = false;
    m_fRotationSpeed = 1.f;
    m_iPassIndex = 1;

    g_fTestDeltaTime = 1.f;

    if (nullptr != pArg)
    {
        CEffect::Effect_Desc* pDesc = static_cast<CEffect::Effect_Desc*>(pArg);

        m_pSocketMatrix = pDesc->pSocketMatrix;
        m_pParentTransformCom = pDesc->pParentTransform;
    }

    return S_OK;
}

void CTest_Ground_Effect::Priority_Update(_float fTimeDelta)
{

}

void CTest_Ground_Effect::Update(_float fTimeDelta)
{
    m_vLifeTime.x += fTimeDelta;


    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));

    if (m_isRotation)
    {
        _vector vCurRotaiton = XMVectorLerp(XMLoadFloat3(&m_vStratRotation), XMLoadFloat3(&m_vStratRotation), m_vLifeTime.x / m_vLifeTime.y);

        m_pTransformCom->Rotation(
            XMConvertToRadians(vCurRotaiton.m128_f32[0]),
            XMConvertToRadians(vCurRotaiton.m128_f32[1]),
            XMConvertToRadians(vCurRotaiton.m128_f32[2]));
    }

    m_pVIBufferCom->Set_Desc(m_isLoop, m_vSize, m_vLifeTime);

    m_pVIBufferCom->Scaling(fTimeDelta, m_eLerpType, m_vPivot, m_fSpeed);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())
        * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CTest_Ground_Effect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CTest_Ground_Effect::Render()
{
#ifdef _DEBUG
    ImGui::InputFloat("DeltaTime", (_float*)(&g_fTestDeltaTime));

    const char* LerpNames[] = { "Linear", "EaseIn", "EaseOut", "EaseInOut" };
    int currentIndex = static_cast<int>(m_eLerpType);

    ImGui::Checkbox("isLoop", &m_isLoop);
    ImGui::Checkbox("isRotate", &m_isRotation);
    ImGui::InputFloat("RotationSpeed", &m_fRotationSpeed);
    ImGui::InputFloat3("Start Rotation", (_float*)(&m_vStratRotation));
    ImGui::InputFloat3("End Rotation", (_float*)(&m_vEndRotation));
    ImGui::InputFloat3("Pivot", (_float*)(&m_vPivot));
    ImGui::InputFloat("Speed", (_float*)(&m_fSpeed));
    if (ImGui::Combo("LerpType", &currentIndex, LerpNames, IM_ARRAYSIZE(LerpNames)))
    {
        m_eLerpType = static_cast<LERP>(currentIndex);
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
            if (ImGui::ImageButton(to_string(iIndex + 100).c_str(), (ImTextureRef)Pair.second, ImVec2(60.f, 60.f)))
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

    ImGui::InputFloat2("vSize", (_float*)&m_vSize);
    ImGui::InputFloat3("vPosition", (_float*)&m_vPosition);
    ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
    m_vLifeTime.y = max(m_vLifeTime.y, 0.01f);
    ImGui::InputInt("Pass", &m_iPassIndex, 1, 0, 5);

    ImGui::Spacing();
    if (ImGui::Button("Save"))
    {
        EFFECT_GROUND_DESC Desc = {};
        EFFECT_GROUND_DATA Data= {};

        Data.eLerpType = m_eLerpType;
        Data.isRotation = m_isRotation;
        Data.iID = m_iEffectID;
        Data.iPassIndex = m_iPassIndex;

        Data.fSpeed = m_fSpeed;

        Data.vLifeTime = _float2(0.f, m_vLifeTime.y);
        Data.vSize = m_vSize;

        Data.vPosition = m_vPosition;
        Data.vPivot = m_vPivot;
        Data.vStartRotation = m_vStratRotation;
        Data.vEndRotation = m_vEndRotation;

        Desc.Ground_Data = Data;
        Desc.strDiffuseTexture = m_strDiffuseTexture;
        Desc.strMaskTexture = m_strMaskTexture;
        Desc.strNoiseTexture = m_strNoiseTexture;

        if (FAILED(CGameManager::GetInstance()->Save_Effect(EFFECT::GROUND, &Desc, "../Bin/Resources/Data/Effect/Player_GroundEffects.xml", m_iEffectID)))
            MSG_BOX("저장 실패");
    }

#endif // _DEBUG

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

    if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTest_Ground_Effect::Add_Components()
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

CTest_Ground_Effect* CTest_Ground_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTest_Ground_Effect* pInstance = new CTest_Ground_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTest_Ground_Effect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CTest_Ground_Effect::Clone(void* pArg)
{
    CGameObject* pInstance = new CTest_Ground_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTest_Ground_Effect");
        return nullptr;
    }

    return pInstance;
}

void CTest_Ground_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);
    Safe_Release(m_pVIBufferCom);
}
