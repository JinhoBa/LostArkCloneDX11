#include "pch.h"
#include "Kamen_Area.h"

#include "GameInstance.h"

CKamen_Area::CKamen_Area(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CKamen_Area::CKamen_Area(const CKamen_Area& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CKamen_Area::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CKamen_Area::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(30.f, 0.f, 30.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(0.35f, 0.35f, 0.35f));
    return S_OK;
}

void CKamen_Area::Priority_Update(_float fTimeDelta)
{

}

void CKamen_Area::Update(_float fTimeDelta)
{
    m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 0.1f);
}

void CKamen_Area::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CKamen_Area::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }


    return S_OK;
}

HRESULT CKamen_Area::Add_Components()
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
CKamen_Area* CKamen_Area::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CKamen_Area* pInstance = new CKamen_Area(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CKamen_Area");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CKamen_Area::Clone(void* pArg)
{
    CGameObject* pInstance = new CKamen_Area(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CKamen_Area");
        return nullptr;
    }

    return pInstance;
}
void CKamen_Area::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
