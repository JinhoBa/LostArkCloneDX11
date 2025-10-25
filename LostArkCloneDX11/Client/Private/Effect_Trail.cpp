#include "pch.h"
#include "Effect_Trail.h"

#include "GameInstance.h"

CEffect_Trail::CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CEffect_Trail::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    TRAIL_EFFECT_DESC* pDesc = static_cast<TRAIL_EFFECT_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentTransformCom = pDesc->pParentTransform;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(2.f, 0.f, 0.f, 1.f));
    
    XMStoreFloat4x4(&m_IdentityMatrix, XMMatrixIdentity());

    m_vSize = _float2(0.1f, 1.f);
    m_vCenter = _float3(0.f, 0.f, 0.f);
    m_vSpeed = _float2(0.1f, 0.2f);
    m_vRange = _float3(0.f, 0.f, 0.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPivot = _float3(0.f, 0.f, 0.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);

    m_iPassIndex = 3;

    return S_OK;
}

void CEffect_Trail::Priority_Update(_float fTimeDelta)
{

}

void CEffect_Trail::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pSocketMatrix) *XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
    if(m_pGameInstance->Get_KeyPressing(DIK_D))
    {
         m_pVIBufferCom->Add_Position(&m_CombinedWorldMatrix);
         m_fWidth = m_pVIBufferCom->Trail(fTimeDelta);
    }
    else
        m_pVIBufferCom->Clear();
}

void CEffect_Trail::Late_Update(_float fTimeDelta)
{
    //m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect_Trail::Render()
{
#ifdef _DEBUG
    const char* LerpNames[] = { "Linear", "EaseIn", "EaseOut", "EaseInOut" };
    int currentIndex = static_cast<int>(m_eLerpType);

    ImGui::Checkbox("isLoop", &m_isLoop);
    if (ImGui::Combo("LerpType", &currentIndex, LerpNames, IM_ARRAYSIZE(LerpNames)))
    {
        m_eLerpType = static_cast<LERP>(currentIndex);
    }
    ImGui::InputFloat2("vSize", (_float*)&m_vSize);
    ImGui::InputFloat3("vPosition", (_float*)&m_vPosition);
    ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
    ImGui::DragInt("Pass", &m_iPassIndex, 1, 0, 3);
    ImGui::DragInt("Texture", &m_iTextureIndex, 1, 0, 2);

    if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (_uint i = 0; i < 5; i++)
        {
            string id = "##Base" + to_string(i);
            if (ImGui::ImageButton(id.c_str(), m_pTextureCom->Get_SRV(i), ImVec2(100.f, 100.f)))
                m_BaseIndex = i;
            if (4 != i % 5)
                ImGui::SameLine();
        }
    }
    ImGui::Spacing();
   
#endif // _DEBUG

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_IdentityMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fWidth", &m_fWidth, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_BaseIndex))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Trail::Add_Components()
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Line_Instance_TrailEffect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    ///*Texture*/
    //if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_decal"),
    //    TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
    //    return E_FAIL;
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Base"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxLineTrail"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CEffect_Trail* CEffect_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Trail* pInstance = new CEffect_Trail(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_Trail");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_Trail(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_Trail");
        return nullptr;
    }

    return pInstance;
}

void CEffect_Trail::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}
