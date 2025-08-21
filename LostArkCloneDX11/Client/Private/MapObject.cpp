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

    m_strPrototypeTag = pDesc->strPrototypeTag;
    memcpy(&m_vPosition, &pDesc->vPosition, sizeof(_float3));
    memcpy(&m_vScale, &pDesc->vScale, sizeof(_float3));
    memcpy(&m_vRotation, &pDesc->vRotation, sizeof(_float3));

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f) );
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(m_vRotation.x, m_vRotation.y, m_vRotation.z);

    m_iSeletPass = 0;

    return S_OK;
}

void CMapObject::Priority_Update(_float fTimeDelta)
{

}

void CMapObject::Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Set_Scale(m_vScale);
    m_pTransformCom->Rotation(XMConvertToRadians(m_vRotation.x), XMConvertToRadians(m_vRotation.y), XMConvertToRadians(m_vRotation.z));


    
}

void CMapObject::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CMapObject::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_iSeletPass)))
        return E_FAIL;

   /* if (FAILED(m_pModelCom->Render(m_pShaderCom)))
        return E_FAIL;*/

   
    m_iSeletPass = 0;

    return S_OK;
}

void CMapObject::Update_ImGui()
{
    if (m_isDead)
        return;
    ImGui::Text(m_pGameInstance->WstringToUtf8(m_strPrototypeTag).c_str());
    ImGui::Text("----- Transfrom ----");
    ImGui::InputFloat("X##Position", &m_vPosition.x, 1.f, 10.f);
    ImGui::InputFloat("Y##Position", &m_vPosition.y, 1.f, 10.f);
    ImGui::InputFloat("Z##Position", &m_vPosition.z, 1.f, 10.f);

    ImGui::Text("----- Rotation ----");
    ImGui::InputFloat("X##Rotation", &m_vRotation.x, 1.f, 10.f);
    ImGui::InputFloat("Y##Rotation", &m_vRotation.y, 1.f, 10.f);
    ImGui::InputFloat("Z##Rotation", &m_vRotation.z, 1.f, 10.f);

    ImGui::Text("----- Scale ----");
    ImGui::InputFloat("X##Scale", &m_vScale.x, 0.1f, 1.f);
    ImGui::InputFloat("Y##Scale", &m_vScale.y, 0.1f, 1.f);
    ImGui::InputFloat("Z##Scale", &m_vScale.z, 0.1f, 1.f);


    if (ImGui::Button("Delete"))
        m_isDead = true;

    m_iSeletPass = 1;
}

_wstring& CMapObject::Get_PrototypeTag()
{
    return m_strPrototypeTag;
}

HRESULT CMapObject::Add_Components()
{

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /*Model*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), m_strPrototypeTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}
CMapObject* CMapObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMapObject* pInstance = new CMapObject(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CMapObject");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CMapObject::Clone(void* pArg)
{
    CGameObject* pInstance = new CMapObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CMapObject");
        return nullptr;
    }

    return pInstance;
}
void CMapObject::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
