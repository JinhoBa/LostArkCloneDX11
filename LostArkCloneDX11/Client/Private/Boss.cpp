#include "pch.h"
#include "Boss.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CBoss::CBoss(const CBoss& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CBoss::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 5.f;
    Desc.fSpeedPersec = 5.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_iAnimIndex = 0;


    return S_OK;
}

void CBoss::Priority_Update(_float fTimeDelta)
{

}

void CBoss::Update(_float fTimeDelta)
{
    for (auto& pSkill : m_Skills)
        pSkill->Update(fTimeDelta);


    m_pModelCom->Play_Animation(m_iAnimIndex, fTimeDelta);

}

void CBoss::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBoss::Render()
{

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
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        
       /* if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;*/


        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;


        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CBoss::Add_Components()
{
    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenBase"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Shader_VTXAnimTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss::Ready_Skills()
{
    for (_uint i = 0; i < 14; ++i)
    {
        CSkill* pSkill = CSkill::Create(i);

        if (nullptr == pSkill)
            return E_FAIL;

        m_Skills.push_back(pSkill);
    }
    return S_OK;
}

CBoss* CBoss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoss* pInstance = new CBoss(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CBoss");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CBoss::Clone(void* pArg)
{
    CGameObject* pInstance = new CBoss(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CBoss");
        return nullptr;
    }

    return pInstance;
}

void CBoss::Free()
{
    __super::Free();

    for (auto pSkill : m_Skills)
        Safe_Release(pSkill);
    m_Skills.clear();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
