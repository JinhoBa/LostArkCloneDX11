#include "pch.h"
#include "Body_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"

CBody_Kamen::CBody_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CBody_Kamen::CBody_Kamen(const CBody_Kamen& Prototype)
    :CPartObject{ Prototype }
{

}

const _float4x4* CBody_Kamen::Get_BoneMatrixPtr(const _char* pBoneName) const
{
    return m_pModelCom->Get_BoneMatrixPrt(pBoneName);
}

HRESULT CBody_Kamen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Kamen::Initialize(void* pArg)
{
    //if (nullptr == pArg)
    //    return E_FAIL;

    //BODYMONSTER_DESC* pDesc = static_cast<BODYMONSTER_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iAnimIndex = 0;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, 0, true);

    return S_OK;
}

void CBody_Kamen::Priority_Update(_float fTimeDelta)
{
}

void CBody_Kamen::Update(_float fTimeDelta)
{
    m_isAnimationFinish = m_pModelCom->Play_Animation(fTimeDelta);

    /* 부모 행렬 적용 */
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));
}

void CBody_Kamen::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Kamen::Render()
{
//#pragma region ANIMATION_TEST
//    ImGui::InputInt("Animation", &m_iAnimIndex);
//    _int iIndex = {};
//    for (auto pName : m_pModelCom->Get_AnimationNames())
//    {
//        if (ImGui::Button(to_string(iIndex).c_str()))
//        {
//            m_iAnimIndex = iIndex;
//            m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, m_iAnimIndex, true);
//        }
//        ++iIndex;
//        ImGui::SameLine();
//        ImGui::Text(pName);
//    }
//#pragma endregion

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))));

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

  /*      if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;*/


        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;


        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

void CBody_Kamen::Set_Animation(_uint iAnimationIdex, _bool bLoop, _float fLerpTime)
{
    m_isAnimationFinish = false;

    m_pModelCom->Set_AnimationIndex(m_pParentTransformCom, iAnimationIdex, bLoop, fLerpTime);
}

HRESULT CBody_Kamen::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenBase"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Shader_VTXAnimTex */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Kamen::Bind_ShaderResources()
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

    return S_OK;
}

CBody_Kamen* CBody_Kamen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Kamen* pInstance = new CBody_Kamen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CBody_Kamen");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CBody_Kamen::Clone(void* pArg)
{
    CGameObject* pInstance = new CBody_Kamen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CBody_Kamen");
        return nullptr;
    }

    return pInstance;
}

void CBody_Kamen::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
