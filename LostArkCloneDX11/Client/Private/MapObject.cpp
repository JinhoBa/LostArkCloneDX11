#include "pch.h"
#include "MapObject.h"

#include "GameInstance.h"

CMapObject::CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CMapObject::CMapObject(const CMapObject& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CMapObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMapObject::Initialize(void* pArg)
{
    MAPOBJECT_DESC* pDesc = static_cast<MAPOBJECT_DESC*>(pArg);

    memcpy(&m_vPosition, &pDesc->vPosition, sizeof(_float3));
    memcpy(&m_vScale, &pDesc->vPosition, sizeof(_float3));
    memcpy(&m_vRotation, &pDesc->vPosition, sizeof(_float3));

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f) );
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(m_vRotation.x, m_vRotation.y, m_vRotation.z);

    return S_OK;
}

void CMapObject::Priority_Update(_float fTimeDelta)
{

}

void CMapObject::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(m_vRotation.x, m_vRotation.y, m_vRotation.z);

}

void CMapObject::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CMapObject::Render()
{
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

    if (FAILED(m_pModelCom->Render()))
        return E_FAIL;

   


    return S_OK;
}

HRESULT CMapObject::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Test"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

void CMapObject::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
