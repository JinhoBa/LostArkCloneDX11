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

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    return S_OK;
}

void CWeapon_Player::Priority_Update(_float fTimeDelta)
{

}

void CWeapon_Player::Update(_float fTimeDelta)
{

}

void CWeapon_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWeapon_Player::Render()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;


        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;


        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeapon_Player::Add_Components()
{
    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*Shader_VTXAnimTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxMesh"),
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
    Safe_Release(m_pModelCom);

}
