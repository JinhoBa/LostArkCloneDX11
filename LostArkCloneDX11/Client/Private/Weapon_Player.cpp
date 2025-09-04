#include "pch.h"
#include "Weapon_Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CWeapon_Player::CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CWeapon_Player::CWeapon_Player(const CWeapon_Player& Prototype)
    :CPartObject{ Prototype }
{
}

HRESULT CWeapon_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWeapon_Player::Initialize(void* pArg)
{
    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pStance = pDesc->pStance;
    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh[ENUM_TO_INT(STANCE::FLURRY)] = m_pModelCom[ENUM_TO_INT(STANCE::FLURRY)]->Get_NumMeshes();
    m_iNumMesh[ENUM_TO_INT(STANCE::FOCUS)] = m_pModelCom[ENUM_TO_INT(STANCE::FOCUS)]->Get_NumMeshes();

    return S_OK;
}

void CWeapon_Player::Priority_Update(_float fTimeDelta)
{

}

void CWeapon_Player::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CWeapon_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWeapon_Player::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh[ENUM_TO_INT(*m_pStance)]; i++)
    {
        if (FAILED((m_pModelCom[ENUM_TO_INT(*m_pStance)])->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;

        if (FAILED((m_pModelCom[ENUM_TO_INT(*m_pStance)])->Bind_Material(i, m_pShaderCom, "g_EmissiveTexture", TEXTURE::EMISSIVE, 0, "g_Emissive_Color")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(2)))
            return E_FAIL;

        if (FAILED(m_pModelCom[ENUM_TO_INT(*m_pStance)]->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeapon_Player::Add_Components()
{
    /*Model_Lance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player_Lance"),
        TEXT("Com_LanceModel"), reinterpret_cast<CComponent**>(&m_pModelCom[ENUM_TO_INT(STANCE::FLURRY)]))))
        return E_FAIL;

    /*Model_Spear*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player_Spear"),
        TEXT("Com_SpearModel"), reinterpret_cast<CComponent**>(&m_pModelCom[ENUM_TO_INT(STANCE::FOCUS)]))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


CWeapon_Player* CWeapon_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Player* pInstance = new CWeapon_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CWeapon_Player");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CWeapon_Player::Clone(void* pArg)
{
    CGameObject* pInstance = new CWeapon_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CWeapon_Player");
        return nullptr;
    }

    return pInstance;
}

void CWeapon_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
 
    Safe_Release(m_pModelCom[ENUM_TO_INT(STANCE::FOCUS)]);
    Safe_Release(m_pModelCom[ENUM_TO_INT(STANCE::FLURRY)]);
}
