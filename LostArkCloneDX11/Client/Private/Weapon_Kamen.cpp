#include "pch.h"
#include "Weapon_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"

CWeapon_Kamen::CWeapon_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CWeapon_Kamen::CWeapon_Kamen(const CWeapon_Kamen& Prototype)
    :CPartObject{ Prototype }
{
}

HRESULT CWeapon_Kamen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWeapon_Kamen::Initialize(void* pArg)
{
    WEAPON_KAMEN_DESC* pDesc = static_cast<WEAPON_KAMEN_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pTransformCom->Rotation(-90.f, 0.f, 0.f);

    return S_OK;
}

void CWeapon_Kamen::Priority_Update(_float fTimeDelta)
{

}

void CWeapon_Kamen::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CWeapon_Kamen::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWeapon_Kamen::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &m_pGameInstance->Get_LightDesc(0).vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &m_pGameInstance->Get_LightDesc(0).vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &m_pGameInstance->Get_LightDesc(0).vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &m_pGameInstance->Get_LightDesc(0).vSpecular, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED((m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor"))))
            return E_FAIL;

        if (FAILED((m_pModelCom->Bind_Material(i, m_pShaderCom, "g_EmissiveTexture", TEXTURE::EMISSIVE, 0, "g_Emissive_Color"))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(2)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeapon_Kamen::Add_Components()
{
    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Weapon_Kamen"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


CWeapon_Kamen* CWeapon_Kamen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Kamen* pInstance = new CWeapon_Kamen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CWeapon_Kamen");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CWeapon_Kamen::Clone(void* pArg)
{
    CGameObject* pInstance = new CWeapon_Kamen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CWeapon_Kamen");
        return nullptr;
    }

    return pInstance;
}

void CWeapon_Kamen::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
