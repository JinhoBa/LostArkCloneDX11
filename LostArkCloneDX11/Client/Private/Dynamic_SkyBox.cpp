#include "pch.h"
#include "Dynamic_SkyBox.h"

#include "GameInstance.h"

CDynamic_SkyBox::CDynamic_SkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CDynamic_SkyBox::CDynamic_SkyBox(const CDynamic_SkyBox& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CDynamic_SkyBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDynamic_SkyBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.CullMode = D3D11_CULL_NONE; // or D3D11_CULL_FRONT, D3D11_CULL_NONE
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = FALSE;

    m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    return S_OK;
}

void CDynamic_SkyBox::Priority_Update(_float fTimeDelta)
{

}

void CDynamic_SkyBox::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(m_pGameInstance->Get_Camera_Position()));

    m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 0.01f);
}

void CDynamic_SkyBox::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CDynamic_SkyBox::Render()
{
    m_pContext->RSSetState(m_pRasterState);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh; i++)
    {

        if (FAILED(m_pShaderCom->Begin(4)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    m_pContext->RSSetState(nullptr);

    return S_OK;
}

HRESULT CDynamic_SkyBox::Add_Components()
{

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_DynamicSky"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_skybox_eclipse"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


    return S_OK;
}
CDynamic_SkyBox* CDynamic_SkyBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDynamic_SkyBox* pInstance = new CDynamic_SkyBox(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CDynamic_SkyBox");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CDynamic_SkyBox::Clone(void* pArg)
{
    CGameObject* pInstance = new CDynamic_SkyBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CDynamic_SkyBox");
        return nullptr;
    }

    return pInstance;
}
void CDynamic_SkyBox::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pRasterState);
}
