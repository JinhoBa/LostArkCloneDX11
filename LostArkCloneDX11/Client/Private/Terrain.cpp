#include "pch.h"
#include "Terrain.h"

#include "GameInstance.h"
#include "GameManager.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice, pContext}
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
    :CGameObject{Prototype}
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.CullMode = D3D11_CULL_BACK; // or D3D11_CULL_FRONT, D3D11_CULL_NONE
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.FrontCounterClockwise = FALSE;


    m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
    m_pPickingPos = {};

    CGameManager::GetInstance()->Bind_PickingPos(&m_pPickingPos);

    return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
    {
        if (m_pVIBufferCom->Picking(m_pTransformCom, &m_pPickingPos))
            int i = 0;
    }
}

void CTerrain::Update(_float fTimeDelta)
{
    


}

void CTerrain::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
#pragma region TESTCODE
    ImGui::Begin("Picking Position");
    ImGui::Text("Position : ");
    ImGui::SameLine();
    ImGui::InputFloat3("", &m_pPickingPos.x);
    ImGui::End();
#pragma endregion
    m_pContext->RSSetState(m_pRasterState);

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pContext->RSSetState(nullptr);

    

    return S_OK;
}
HRESULT CTerrain::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXNorTex"),
        TEXT("Com_Shader_VTXNorTex"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTerrain* pInstance = new CTerrain(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTerrain");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
    CGameObject* pInstance = new CTerrain(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTerrain");
        return nullptr;
    }

    return pInstance;
}

void CTerrain::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);

    Safe_Release(m_pRasterState);
}
