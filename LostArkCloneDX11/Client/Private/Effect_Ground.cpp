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

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.1f, 0.f, 1.f));


    m_vSize = _float2(0.1f, 1.f);
    m_vCenter = _float3(0.f, 0.f, 0.f);
    m_vSpeed = _float2(0.1f, 0.2f);
    m_vRange = _float3(0.f, 0.f, 0.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPivot = _float3(0.f, 0.f, 0.f);
    m_vPosition = _float3(45.f, 0.f, 45.f);

    m_iPassIndex = 3;

    return S_OK;
}

void CEffect_Ground::Priority_Update(_float fTimeDelta)
{

}

void CEffect_Ground::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pVIBufferCom->Set_Desc(m_isLoop, m_vSize, m_vCenter, m_vRange, m_vLifeTime);

    m_pVIBufferCom->Scaling(fTimeDelta, m_eLerpType);
}

void CEffect_Ground::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect_Ground::Render()
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
    ImGui::InputFloat3("vPosition", (_float*) &m_vPosition);
    ImGui::InputFloat2("vLifeTime", (_float*)&m_vLifeTime);
    ImGui::DragInt("Pass", &m_iPassIndex, 1, 0, 3);
    ImGui::DragInt("Texture", &m_iTextureIndex, 1, 0, 2);
#endif // _DEBUG


    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iTextureIndex))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Ground::Add_Components()
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_GroundEffect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    ///*Texture*/
    //if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_decal"),
    //    TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
    //    return E_FAIL;
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_hit"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPointParticle"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

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
    Safe_Release(m_pVIBufferCom);
}
