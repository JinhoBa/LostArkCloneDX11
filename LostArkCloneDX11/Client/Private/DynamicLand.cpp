#include "pch.h"
#include "DynamicLand.h"

#include "GameInstance.h"
#include "GameManager.h"

CDynamicLand::CDynamicLand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CDynamicLand::CDynamicLand(const CDynamicLand& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CDynamicLand::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDynamicLand::Initialize(void* pArg)
{

    m_vPosition = _float3(30.f, 0.1f, 30.f);
    m_vScale = _float3(1.5f, 1.5f, 1.5f);
    m_vRotation = _float3(0.f, 0.f, 90.f);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(m_vRotation.x, m_vRotation.y, m_vRotation.z);

    m_iSeletPass = 0;

    m_vLifeTime = _float2(0.f, 10.f);
    m_fSpped = 0.008f;
    m_vColor = _float4(0.57f, 0.57f, 1.f, 1.f);
    m_fNoiseValue = 0.031f;
    m_fSplit = 1.37f;
    return S_OK;
}

void CDynamicLand::Priority_Update(_float fTimeDelta)
{

}

void CDynamicLand::Update(_float fTimeDelta)
{
    m_vLifeTime.x += fTimeDelta * m_fSpped;

    if (m_vLifeTime.x > m_vLifeTime.y)
        m_vLifeTime.x = 0.f;

#ifdef _DEBUG
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));
#endif // _DEBUG
}

void CDynamicLand::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDynamicLand::Render()
{
    Update_ImGui();

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fSplit", &m_fSplit, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fNoiseValue", &m_fNoiseValue, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_DiffuseColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pNoiseTextureCom->Get_SRV(0))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeRatio", &m_vLifeTime.x, sizeof(_float))))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Begin(8)))
        return E_FAIL;

    if (FAILED(m_pModelCom->Render(0)))
        return E_FAIL;

    return S_OK;
}


void CDynamicLand::Update_ImGui()
{

    ImGui::Text(m_pGameInstance->WstringToUtf8(m_strPrototypeTag).c_str());
    ImGui::SliderFloat("NoiseSpeed", &m_fSpped, 0.f, 1.f);
    ImGui::SliderFloat("Split", &m_fSplit, 0.f, 100.f);
    ImGui::SliderFloat("NoiseValue", &m_fNoiseValue, 0.f, 1.f);
    ImGui::InputFloat3("Color", (_float*)(&m_vColor));

    ImGui::Text("----- Transfrom ----");
    ImGui::InputFloat("X##Position", &m_vPosition.x, 1.f, 10.f);
    ImGui::InputFloat("Y##Position", &m_vPosition.y, 0.1f, 1.f);
    ImGui::InputFloat("Z##Position", &m_vPosition.z, 1.f, 10.f);

    ImGui::Text("----- Rotation ----");
    ImGui::InputFloat("X##Rotation", &m_vRotation.x, 1.f, 10.f);
    ImGui::InputFloat("Y##Rotation", &m_vRotation.y, 1.f, 10.f);
    ImGui::InputFloat("Z##Rotation", &m_vRotation.z, 1.f, 10.f);

    ImGui::Text("----- Scale ----");
    ImGui::InputFloat("X##Scale", &m_vScale.x, 0.1f, 1.f);
    ImGui::InputFloat("Y##Scale", &m_vScale.y, 0.1f, 1.f);
    ImGui::InputFloat("Z##Scale", &m_vScale.z, 0.1f, 1.f);
}

_wstring& CDynamicLand::Get_PrototypeTag()
{
    return m_strPrototypeTag;
}

HRESULT CDynamicLand::Add_Components()
{

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane1"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Water"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_WaterNoise"),
        TEXT("Com_NoiseTexture"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    return S_OK;
}
CDynamicLand* CDynamicLand::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDynamicLand* pInstance = new CDynamicLand(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CDynamicLand");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CDynamicLand::Clone(void* pArg)
{
    CGameObject* pInstance = new CDynamicLand(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CDynamicLand");
        return nullptr;
    }

    return pInstance;
}
void CDynamicLand::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pNoiseTextureCom);
}
