#include "pch.h"
#include "SkySphere.h"

#include "GameInstance.h"

CSkySphere::CSkySphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CSkySphere::CSkySphere(const CSkySphere& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CSkySphere::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkySphere::Initialize(void* pArg)
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

    
    m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));

    return S_OK;
}

void CSkySphere::Priority_Update(_float fTimeDelta)
{

}

void CSkySphere::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(m_pGameInstance->Get_Camera_Position()));


}

void CSkySphere::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CSkySphere::Render()
{
    m_pContext->RSSetState(m_pRasterState);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pModelCom->Render(m_pShaderCom)))
        return E_FAIL;

    m_pContext->RSSetState(nullptr);

    return S_OK;
}

HRESULT CSkySphere::Add_Components()
{

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Sky"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}
CSkySphere* CSkySphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkySphere* pInstance = new CSkySphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CSkySphere");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CSkySphere::Clone(void* pArg)
{
    CGameObject* pInstance = new CSkySphere(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CSkySphere");
        return nullptr;
    }

    return pInstance;
}
void CSkySphere::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pRasterState);
}
