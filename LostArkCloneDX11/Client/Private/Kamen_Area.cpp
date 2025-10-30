#include "pch.h"
#include "Kamen_Area.h"

#include "GameInstance.h"

#include "Effect.h"


CKamen_Area::CKamen_Area(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject{ pDevice, pContext }
{
}

CKamen_Area::CKamen_Area(const CKamen_Area& Prototype)
    :CContainerObject{ Prototype }
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

    CEffect::EFFECT_DESC Effect_Desc = {};

    Effect_Desc.pParentTransform = m_pTransformCom;
    Effect_Desc.pSocketMatrix = &m_pTransformCom->Get_WorldMatrix();

    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Kamen_Area_Effect"), TEXT("Test_Effect"), &Effect_Desc)))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(30.f, 22.9f, 30.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(0.41f, 0.41f, 0.41f));

    return S_OK;
}

void CKamen_Area::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CKamen_Area::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    m_fTimeAcc += fTimeDelta;

    if (100.f <= m_fTimeAcc)
        m_fTimeAcc = 0.f;
}

void CKamen_Area::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    m_pGameInstance->Add_RenderGroup(RENDER::NONLIGHT, this);
}

HRESULT CKamen_Area::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeRatio", &m_fTimeAcc, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;


    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pShaderCom->Begin(9)))
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
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_KamenBack"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenEmssive"),
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
