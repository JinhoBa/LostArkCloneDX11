#include "pch.h"
#include "Test_Effect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"
#include "Kamen.h"

CTest_Effect::CTest_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CTest_Effect::CTest_Effect(const CTest_Effect& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CTest_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTest_Effect::Initialize(void* pArg)
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

    m_iNumInstance = 50;
    m_vSize = _float2(0.05f, 0.1f);
    m_vCenter = _float3(0.f, 0.f, 0.f);
    m_vSpeed = _float2(3.f, 5.f);
    m_vRange = _float3(1.f, 1.f, 1.f);
    m_vLifeTime = _float2(0.5f, 0.5f);
    m_vPivot = _float3(0.f, 0.f, 0.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_vRotation = _float3(0.f, 0.f, 0.f);

    m_isActive = false;
    m_fTimeAcc = 0.f;

    m_fSizeX = 1.f;
    m_fSizeY = 1.f;

    XMStoreFloat4x4(&m_ParentWorldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

    m_isBlur = true;

    m_fRotationSpeed = 1.f;
    m_fRotationAngle = 0.f;


    return S_OK;
}

void CTest_Effect::Priority_Update(_float fTimeDelta)
{

}

void CTest_Effect::Update(_float fTimeDelta)
{
    //XMStoreFloat3(&m_vPosition, m_pParentTransformCom->Get_Position());
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f));

    m_fRotationAngle += m_fRotationSpeed * fTimeDelta;
    if (m_fRotationAngle >= 360.f)
        m_fRotationAngle = 0.f;

    _matrix matRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));

    switch (m_eType)
    {
    case Engine::PARTICLE::SPREAD:
        m_pVIBufferCom->Spread(m_vCenter, m_vRange, fTimeDelta);
        break;
    case Engine::PARTICLE::SCALE:

        break;
    case Engine::PARTICLE::CIRCLE:
        m_pVIBufferCom->Circle(m_vCenter, m_vRange, fTimeDelta);
        break;

    case Engine::PARTICLE::ROUND:
        m_pVIBufferCom->Round(m_vCenter, m_vRange, fTimeDelta);
        break;

    case Engine::PARTICLE::CORN:
        m_pVIBufferCom->Corn(m_vCenter, m_vRange, fTimeDelta);
        break;

    default:
        break;
    }
    

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * matRotation * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CTest_Effect::Late_Update(_float fTimeDelta)
{
   m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
   m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CTest_Effect::Render()
{
    if (m_isBlur)
    {
#ifdef _DEBUG
        if (ImGui::Button("Change"))
        {
            m_pVIBufferCom->Set_NumInstance(m_iNumInstance);
            if(m_eType == PARTICLE::SPREAD)
                m_pVIBufferCom->Set_Spread(m_isLoop, m_vCenter, m_vPivot, m_vRange, m_vLifeTime, m_vSpeed, m_vSize);
            else if(m_eType == PARTICLE::CIRCLE)
                m_pVIBufferCom->Set_Circle(m_isLoop, m_vCenter, m_vPivot, m_vRange, m_vLifeTime, m_vSpeed, m_vSize);
            else if(m_eType == PARTICLE::ROUND)
                m_pVIBufferCom->Set_Round(m_isLoop, m_vCenter, m_vPivot, m_vRange, m_vLifeTime, m_vSpeed, m_vSize);
            else if(m_eType == PARTICLE::CORN)
                m_pVIBufferCom->Set_Corn(m_isLoop, m_vCenter, m_vPivot, m_vRange, m_vLifeTime, m_vSpeed, m_vSize);
            
        }
        const char* Particle[] = { "Spread", "Scale", "Cirlce", "Round", "Corn" };

        if (ImGui::Combo("Variable", (int*)(&m_eType), Particle, IM_ARRAYSIZE(Particle)))
        {
       
        }
        ImGui::Spacing();
        ImGui::Checkbox("isLoop", &m_isLoop);
        ImGui::InputInt("iNumInstance", (int*)&m_iNumInstance);
        ImGui::InputInt("PassIndex", (int*)&m_iPassIndex);
        ImGui::InputFloat2("vSize", (_float*)&m_vSize);
        ImGui::InputFloat2("vSpeed", (_float*)&m_vSpeed);
        ImGui::InputFloat3("vCenter", (_float*)&m_vCenter);
        ImGui::InputFloat3("vPivot", (_float*)&m_vPivot);
        ImGui::InputFloat3("vRange", (_float*)&m_vRange);
        ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
        ImGui::InputFloat("ActiveTime", (_float*)&m_fActiveTime);
        ImGui::Spacing();
        ImGui::InputFloat3("Position", (_float*)&m_vPosition);
        ImGui::DragFloat3("Rotation", (_float*)&m_vRotation);
        ImGui::DragFloat("RotationSpeed", &m_fRotationSpeed);
        ImGui::InputFloat("TileX", &m_fSizeX);
        ImGui::InputFloat("TileY", &m_fSizeY);

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
        ImGui::Spacing();
        m_isBlur = false;

        if (ImGui::Button("Save"))
        {
            EFFECT_PARTICLE_DESC Desc = {};
            EFFECT_PARTICLE_DATA Data = {};

            Data.eType = m_eType;
            Data.isLoop = m_isLoop;
            Data.iPassIndex = m_iPassIndex;
            Data.iNumInstance = m_iNumInstance;
            Data.fActiveTime = m_fActiveTime;

            Data.vTileSize = _float2(m_fSizeX, m_fSizeY);
            Data.vSize = m_vSize;
            Data.vLifeTime = m_vLifeTime;
            Data.vSpeed = m_vSpeed;

            Data.vCenter = m_vCenter;
            Data.vPivot = m_vPivot;
            Data.vRange = m_vRange;
            Data.vPosition = m_vPosition;
            Data.vRotation = m_vRotation;

            Desc.Data = Data;
            Desc.strDiffuseTexture = m_strDiffuseTexture;
            Desc.strMaskTexture = m_strMaskTexture;
            Desc.strNoiseTexture = m_strNoiseTexture;

            if (FAILED(CGameManager::GetInstance()->Save_Effect(EFFECT::PARTICLE, &Desc, "../Bin/Resources/Data/Effect/Kamen_ParticleEffects.xml", 0)))
                MSG_BOX("저장 실패");
            else
            {
                MSG_BOX("저장 완료");
            }
        }
#endif // _DEBUG
 }
    else {
        m_isBlur = true;
    }

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

HRESULT CTest_Effect::Add_Components()
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


CTest_Effect* CTest_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTest_Effect* pInstance = new CTest_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTest_Effect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CTest_Effect::Clone(void* pArg)
{
    CGameObject* pInstance = new CTest_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTest_Effect");
        return nullptr;
    }

    return pInstance;
}

void CTest_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);
    Safe_Release(m_pVIBufferCom);
}
