#include "pch.h"
#include "WeaponEffect_Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CWeaponEffect_Player::CWeaponEffect_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CWeaponEffect_Player::CWeaponEffect_Player(const CWeaponEffect_Player& Prototype)
    :CPartObject{ Prototype }
{
}

const _float4x4* CWeaponEffect_Player::Get_BoneMatrixPtr(const _char* pBoneName) const
{
    return m_pModelCom->Get_BoneMatrixPrt(pBoneName);
}

void CWeaponEffect_Player::Shoot(_float4* pTargetPosition)
{
    m_eState = STATE::SHOOT;

    _vector vPosition = XMLoadFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[3]));

    m_pTransformCom->Set_State(Engine::STATE::POSITION, vPosition);
    m_pTransformCom->LookAt(XMLoadFloat4(pTargetPosition));

    _vector vRight = m_pTransformCom->Get_State(Engine::STATE::RIGHT);
    _vector vLook = m_pTransformCom->Get_State(Engine::STATE::LOOK);

    /* 이동 방향 */
    XMStoreFloat4(&m_vShootDir, XMVector3Normalize(vLook));

    m_pTransformCom->Set_State(Engine::STATE::RIGHT, vLook);
    m_pTransformCom->Set_State(Engine::STATE::LOOK, vRight * -1.f);

}

HRESULT CWeaponEffect_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWeaponEffect_Player::Initialize(void* pArg)
{
    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_isVisible = false;
    m_fSpeed = 0.3f;
    m_fTimeAcc = 0.f;


    m_fRimStrength = 1.5f;
    m_fRimPower = 1.f;
    m_vRimColor = _float4(0.4f, 0.8f, 1.f, 1.f);

    return S_OK;
}

void CWeaponEffect_Player::Priority_Update(_float fTimeDelta)
{

}

void CWeaponEffect_Player::Update(_float fTimeDelta)
{
    if (m_isVisible)
    {
        m_fTimeAcc += fTimeDelta * 0.8f;
        if(STATE::IDLE == m_eState)
        {
   
            XMStoreFloat4x4(&m_CombinedWorldMatrix,
                XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
        }
        else
        {
            m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pTransformCom->Get_Position() + XMLoadFloat4(&m_vShootDir) * m_fSpeed);

            XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

            if(-5.f > XMVectorGetY(m_pTransformCom->Get_Position()))
            {
                m_eState = STATE::IDLE;
                m_isVisible = false;
                m_fTimeAcc = 0.f;
                m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
                m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
            }
        }
    }
    
}

void CWeaponEffect_Player::Late_Update(_float fTimeDelta)
{
    if(m_isVisible)
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWeaponEffect_Player::Render()
{
#ifdef _DEBUG
    ImGui::InputFloat("EffecSpeed", &m_fSpeed);
#endif // _DEBUG


    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", &m_pGameInstance->Get_Veiwport().MaxDepth, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_NoiseTexture", m_pTextureCom->Get_SRV(3))))
        return E_FAIL;

    _float fRation = m_fTimeAcc;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeRatio", &fRation, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &m_vRimColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimStrength", &m_fRimStrength, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimPower", &m_fRimPower, sizeof(_float))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; ++i)
    {
        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_EmissiveTexture", TEXTURE::EMISSIVE, 0, "g_EmissiveColor")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(6)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeaponEffect_Player::Add_Components()
{
    /*Model_Spear*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player_WeaponEffect"),
        TEXT("Com_EffectModel"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Noise"),
        TEXT("Com_NoiseTexture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}


CWeaponEffect_Player* CWeaponEffect_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeaponEffect_Player* pInstance = new CWeaponEffect_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CWeaponEffect_Player");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CWeaponEffect_Player::Clone(void* pArg)
{
    CGameObject* pInstance = new CWeaponEffect_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CWeaponEffect_Player");
        return nullptr;
    }

    return pInstance;
}

void CWeaponEffect_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pModelCom);
}
