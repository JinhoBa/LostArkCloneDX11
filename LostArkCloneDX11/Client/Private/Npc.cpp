#include "pch.h"
#include "Npc.h"

#include "GameInstance.h"
#include "GameManager.h"

CNpc::CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCharacter{ pDevice, pContext }
{
}

CNpc::CNpc(const CNpc& Prototype)
    :CCharacter{ Prototype }
{
}

HRESULT CNpc::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{

    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 5.f;
    Desc.fSpeedPersec = 3.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_pModelCom->Set_AnimationIndex(m_pTransformCom, 0, true);

    m_pTransformCom->Set_Scale(_float3(1.2f, 1.2f, 1.2f));
    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(40.f, 0.f, 35.f, 1.f));

    m_ePreState = { STATE::END };
    m_eCurState = { STATE::FEAR };
    m_fWalkSpeed = 0.7f;

    m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (nullptr == m_pPlayerTransformCom)
        return E_FAIL;

    Safe_AddRef(m_pPlayerTransformCom);



    return S_OK;
}

void CNpc::Priority_Update(_float fTimeDelta)
{

}

void CNpc::Update(_float fTimeDelta)
{
    Change_State();

    switch (m_eCurState)
    {
    case Client::CNpc::IDLE:
        if (m_pGameInstance->Get_KeyDown(DIK_G) && 
            1.f >= XMVector3Length(m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position()).m128_f32[0])
        {
            m_eCurState = STATE::TALK;
            m_pGameManager->Start_Dialogue(0, m_pTransformCom->Get_Position());
        }
        
        break;

    case Client::CNpc::TALK:
        if (m_pGameInstance->Get_KeyDown(DIK_ESCAPE))
        {
            m_eCurState = STATE::IDLE;
            CGameManager::GetInstance()->End_Dialogue();
        }
        break;

    case Client::CNpc::WALK:
        if (false == m_pTransformCom->MoveTo(fTimeDelta, XMVectorSet(40.f, 0.f, 39.f, 1.f), m_fWalkSpeed, m_pNavigationCom))
            m_eCurState = STATE::IDLE;
        break;

    case Client::CNpc::FEAR:
        if (0 == (_uint)m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Layer_Monster")).size())
        {
            m_eCurState = STATE::WALK;
        }

        break;

    default:
        break;
    }

    m_pModelCom->Play_Animation(fTimeDelta);
}

void CNpc::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CNpc::Render()
{
    //#pragma region ANIMATION_TEST
    //    ImGui::InputInt("Animation", &m_iAnimIndex);
    //    _int iIndex = {};
    //    for (auto pName : m_pModelCom->Get_AnimationNames())
    //    {
    //        if (ImGui::Button(to_string(iIndex).c_str()))
    //        {
    //            m_iAnimIndex = iIndex;
    //            m_pModelCom->Set_AnimationIndex(m_pTransformCom, m_iAnimIndex, true);
    //        }
    //        ++iIndex;
    //        ImGui::SameLine();
    //        ImGui::Text(pName);
    //    }
    //#pragma endregion

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
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

HRESULT CNpc::Ready_Components()
{
    CNavigation::NAVIGATION_DESC NavDesc = {};

    NavDesc.iCurrentIndex = 0;

    /* Navigation */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Trision"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
        return E_FAIL;

    /* AnimModel */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Npc"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Shader_VTXAnimTex */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;



    return S_OK;
}

void CNpc::Change_State()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case Client::CNpc::IDLE:
            m_pModelCom->Set_AnimationIndex(m_pTransformCom, 3, true);
            break;

        case Client::CNpc::TALK:
            m_pModelCom->Set_AnimationIndex(m_pTransformCom, 2, true);
            break;

        case Client::CNpc::FEAR:
            m_pModelCom->Set_AnimationIndex(m_pTransformCom, 16, true);
            break;

        case Client::CNpc::WALK:
            m_pModelCom->Set_AnimationIndex(m_pTransformCom, 18, true);
            break;

        default:
            break;
        }

        m_ePreState = m_eCurState;
    }
}

CNpc* CNpc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNpc* pInstance = new CNpc(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CNpc");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CNpc::Clone(void* pArg)
{
    CGameObject* pInstance = new CNpc(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CNpc");
        return nullptr;
    }

    return pInstance;
}

void CNpc::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerTransformCom);
    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
