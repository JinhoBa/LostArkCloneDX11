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
    if (nullptr != pArg)
    {
        TERRAIN_DESC* pDesc = static_cast<TERRAIN_DESC*>(pArg);
        m_strPrototypeTag = pDesc->strPrototypeTag;
        m_vPosition = _float4(pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z, 1.f);
        m_vRotation = pDesc->vRotation;
    }
    else
    {
        m_strPrototypeTag = L"";
        m_vPosition = _float4(0.f, 0.f, 0.f, 1.f);
        m_vRotation = _float3(0.f, 0.f, 0.f);
    }

    

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

    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&m_vPosition));
    m_pTransformCom->Rotation(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));

    m_bVisible = true;
    m_PosYValue = 0.f;

    return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_DIMousePressing(MOUSEKEYSTATE::RBUTTON))
    {
        
        if (m_pVIBufferCom->Picking(m_pTransformCom, &m_pPickingPos))
            CGameManager::GetInstance()->Bind_PickingPos(&m_pPickingPos);

        if (m_pGameInstance->Get_KeyPressing(DIK_9))
        {
            if (m_pVIBufferCom->Picking_Edit(m_pTransformCom, &m_pPickingPos, 0.1f))
                CGameManager::GetInstance()->Bind_PickingPos(&m_pPickingPos);
        }

        if (m_pGameInstance->Get_KeyPressing(DIK_8))
        {
            if (m_pVIBufferCom->Picking_Edit(m_pTransformCom, &m_pPickingPos, -0.1f))
                CGameManager::GetInstance()->Bind_PickingPos(&m_pPickingPos);
        }

        if (m_pGameInstance->Get_KeyPressing(DIK_LCONTROL))
        {
            if (m_pVIBufferCom->Picking_Smooth(m_pTransformCom, &m_pPickingPos, m_PosYValue))
                CGameManager::GetInstance()->Bind_PickingPos(&m_pPickingPos);
        }
        
    }

}

void CTerrain::Update(_float fTimeDelta)
{
    
    if (m_pGameInstance->Get_KeyDown(DIK_P))
        m_bVisible = !m_bVisible;

    /*m_pVIBufferCom->Change_Verices(m_iSizeX, m_iSizeZ);*/

    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&m_vPosition));
    m_pTransformCom->Rotation(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));
}

void CTerrain::Late_Update(_float fTimeDelta)
{
    if(m_bVisible)
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
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

void CTerrain::Update_ImGui()
{
#pragma region TESTCODE
    if (isDead())
        return;
    ImGui::Text("Position : "); 
    ImGui::SameLine();
    ImGui::InputFloat3("pick", &m_pPickingPos.x);
    ImGui::InputFloat("PickingY", &m_PosYValue, 0.01f, 0.1f);
    ImGui::InputFloat("PosX", &m_vPosition.x, 1.f, 10.f);
    ImGui::InputFloat("PosY", &m_vPosition.y, 1.f, 10.f);
    ImGui::InputFloat("PosZ", &m_vPosition.z, 1.f, 10.f);
    ImGui::InputFloat("RotX", &m_vRotation.x, 1.f, 10.f);
    ImGui::InputFloat("RotY", &m_vRotation.y, 1.f, 10.f);
    ImGui::InputFloat("RotZ", &m_vRotation.z, 1.f, 10.f);
   /* ImGui::InputInt("SizeX", &m_iSizeX, 1.f, 10.f);
    ImGui::InputInt("SizeZ", &m_iSizeZ, 1.f, 10.f);*/
   
    ImGui::Text("Save File Name : ");
    ImGui::InputText("##name", m_szHeightMapFileName, MAX_PATH);

    if (ImGui::Button("Save HeightMap"))
    {
        char szSaveFilePath[MAX_PATH] = {};

        strcpy_s(szSaveFilePath, "../Bin/Resources/HeightMap/");
        strcat_s(szSaveFilePath, m_szHeightMapFileName);
        strcat_s(szSaveFilePath, ".bin");

        m_pVIBufferCom->Save_HeightFile(szSaveFilePath);
    }
   
    if (ImGui::Button("Delete"))
        m_isDead = true;
#pragma endregion
}

HRESULT CTerrain::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), m_strPrototypeTag,
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
