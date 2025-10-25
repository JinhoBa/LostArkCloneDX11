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

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

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
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::EMISSIVE, 0, "g_DiffuseColor")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(2)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

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
}
