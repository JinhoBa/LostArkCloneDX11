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

    m_vSize = _float2(0.1f, 2.f);
    m_vCenter = _float3(0.f, 0.f, 0.f);
    m_vSpeed = _float2(0.1f, 0.2f);
    m_vRange = _float3(0.f, 0.f, 0.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPivot = _float3(0.f, 0.f, 0.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_isRotation = false;
    m_fRotationSpeed = 1.f;
    m_iPassIndex = 3;

    m_BaseIndex = 0;
    m_MaskIndex = 0;

    g_fTestDeltaTime = 1.f;

    m_bTest = true;

    if (nullptr != pArg)
    {
        CEffect::Effect_Desc* pDesc = static_cast<CEffect::Effect_Desc*>(pArg);

        m_pSocketMatrix = pDesc->pSocketMatrix;
        m_pParentTransformCom = pDesc->pParentTransform;
    }

    return S_OK;
}

void CEffect_Ground::Priority_Update(_float fTimeDelta)
{

}

void CEffect_Ground::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Rotation(
        XMConvertToRadians(m_vRotation.x), 
        XMConvertToRadians(m_vRotation.y),
        XMConvertToRadians(m_vRotation.z));

    if(m_isRotation)
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fRotationSpeed * fTimeDelta);

    m_pVIBufferCom->Set_Desc(m_isLoop, m_vSize, m_vCenter, m_vRange, m_vLifeTime);

    m_pVIBufferCom->Scaling(fTimeDelta, m_eLerpType, m_vPivot, m_fSpeed);

   XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())
         * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()) );
}

void CEffect_Ground::Late_Update(_float fTimeDelta)
{

    m_fLifeTime += fTimeDelta;
    if (3.f < m_fLifeTime && false == m_bTest)
        m_isDead = true;
    else
        m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect_Ground::Render()
{
#ifdef _DEBUG
    ImGui::InputFloat("DeltaTime", (_float*)(&g_fTestDeltaTime));

    const char* LerpNames[] = { "Linear", "EaseIn", "EaseOut", "EaseInOut" };
    int currentIndex = static_cast<int>(m_eLerpType);

    ImGui::Checkbox("isLoop", &m_isLoop);
    ImGui::Checkbox("isRotate", &m_isRotation);
    ImGui::InputFloat("RotationSpeed", &m_fRotationSpeed);
    ImGui::InputFloat3("Rotation", (_float*)(&m_vRotation));
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
    ImGui::InputFloat3("vPosition", (_float*) &m_vPosition);
    ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
    ImGui::DragInt("Pass", &m_iPassIndex, 1, 0, 4);
    ImGui::DragInt("Texture", &m_iTextureIndex, 1, 0, 2);


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

    if (FAILED(m_pShaderCom->Begin(4)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Ground::Start(const _float4x4* pWorldMatrix, void* pArg)
{
    m_bTest = false;
    m_fLifeTime = 0.f;
    m_pVIBufferCom->Reset();

    EFFECT_GROUND_DESC* pDesc = static_cast<EFFECT_GROUND_DESC*>(pArg);

    m_vPosition = pDesc->vPosition;
    return S_OK;
}

HRESULT CEffect_Ground::Reset()
{
    m_fLifeTime = 0.f;
    m_pVIBufferCom->Reset();
    m_isDead = false;

    return S_OK;
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

    EFFECT_GROUND_COM* pDesc = static_cast<EFFECT_GROUND_COM*>(pArg);

    m_pTextureCom = pDesc->m_pTextureCom;
    m_pMaskTextureCom = pDesc->m_pMaskTextureCom;
    m_pNoiseTextureCom = pDesc->m_pNoiseTextureCom;

    Safe_AddRef(m_pTextureCom);
    Safe_AddRef(m_pTextureCom);
    Safe_AddRef(m_pTextureCom);

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
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTextureCom);
    Safe_Release(m_pVIBufferCom);
}
