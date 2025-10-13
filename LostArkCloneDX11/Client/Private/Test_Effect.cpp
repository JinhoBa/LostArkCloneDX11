#include "pch.h"
#include "Test_Effect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CTest_Effect::CTest_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect{ pDevice, pContext }
{
}

CTest_Effect::CTest_Effect(const CTest_Effect& Prototype)
    :CEffect{ Prototype }
{
}

HRESULT CTest_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTest_Effect::Initialize(void* pArg)
{
    CEffect::EFFECT_DESC* pDesc = static_cast<EFFECT_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentTransformCom = pDesc->pParentTransform;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    m_isLoop = true;
    m_iNumInstance = 300;
    m_vSize = _float2(0.1f, 1.f);
    m_vCenter = _float3(0.f, 0.f, 0.f);
    m_vSpeed = _float2(0.1f, 0.2f);
    m_vRange = _float3(0.f, 0.f, 0.f);
    m_vLifeTime = _float2(1.f, 1.0f);
    m_vPivot = _float3(1.f, 1.f, 1.f);
    m_vPosition = _float3(0.f, 0.f, 0.f);
    m_vRotation = _float3(0.f, 0.f, 0.f);

    m_isActive = false;
    m_fTimeAcc = 0.f;

    XMStoreFloat4x4(&m_CombindedMatrix, XMMatrixIdentity());

    m_iNumMesh = m_pModelCom->Get_NumMeshes();
    m_iMeshIndex = 0;

    return S_OK;
}

void CTest_Effect::Priority_Update(_float fTimeDelta)
{

}

void CTest_Effect::Update(_float fTimeDelta)
{
    memcpy(&m_vCenter, m_pSocketMatrix->m[3], sizeof(_float3));

 

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vPosition), 1.f));
    m_pTransformCom->Rotation(m_vRotation.x, m_vRotation.y, m_vRotation.z);
    m_pTransformCom->Set_Scale(m_vPivot);

  //  m_pVIBufferCom->Set_Desc(m_isLoop, m_iNumInstance, m_vSize, m_vCenter, m_vSpeed, m_vRange, m_vLifeTime, m_vPivot);
    //m_pVIBufferCom->Spread(fTimeDelta);


    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CTest_Effect::Late_Update(_float fTimeDelta)
{
   m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CTest_Effect::Render()
{
#ifdef _DEBUG
    ImGui::Checkbox("isLoop", &m_isLoop);
    ImGui::InputInt("iNumInstance", (int*) & m_iNumInstance);
    ImGui::InputFloat2("vSize", (_float*) &m_vSize);
    //ImGui::InputFloat3("vCenter", (_float*) &m_vCenter);
    ImGui::InputFloat2("vSpeed", (_float*) &m_vSpeed);
    ImGui::InputFloat3("vRange", (_float*) &m_vRange);
    ImGui::InputFloat2("vLifeTime", (_float*) &m_vLifeTime);
    ImGui::InputFloat3("vPivot", (_float*) &m_vPivot);
    ImGui::InputFloat3("vPosition", (_float*) &m_vPosition);
    ImGui::InputFloat3("vRotation", (_float*) &m_vRotation);

#endif // _DEBUG


    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
    //    return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
        return E_FAIL;



    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

   /* if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;
    
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;*/

    if (FAILED(m_pModelCom->Render(0)))
        return S_OK;

    return S_OK;
}

HRESULT CTest_Effect::Add_Components()
{
    /*VIBuffer_Point_Instance*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_TestEffect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Trail"),
        TEXT("Com_TextureTrail"), reinterpret_cast<CComponent**>(&m_pTextureTrailCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMeshEffect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Shader_VertexMesh */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_TrailMeshes"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}


CTest_Effect* CTest_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTest_Effect* pInstance = new CTest_Effect(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTest_Effect");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CTest_Effect::Clone(void* pArg)
{
    CGameObject* pInstance = new CTest_Effect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTest_Effect");
        return nullptr;
    }

    return pInstance;
}

void CTest_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTextureTrailCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pModelCom);
}
