#include "pch.h"
#include "Body_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Camera.h"

CBody_Kamen::CBody_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CBody_Kamen::CBody_Kamen(const CBody_Kamen& Prototype)
    :CPartObject{ Prototype }
{

}
_float CBody_Kamen::Get_TrackPoisiton()
{ 
    return m_pModelComs[m_iCurModelIndex]->Get_TrackPosition();
}
const _float4x4* CBody_Kamen::Get_BoneMatrixPtr(const _char* pBoneName) const
{
    return m_pModelComs[m_iCurModelIndex]->Get_BoneMatrixPrt(pBoneName);
}

HRESULT CBody_Kamen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Kamen::Initialize(void* pArg)
{
    //if (nullptr == pArg)
    //    return E_FAIL;

    //BODYKAMEN_DESC* pDesc = static_cast<BODYKAMEN_DESC*>(pArg);
    //m_pPhase = pDesc->pPhase;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_iCurModelIndex = 0;
    m_iAnimIndex = 193;

#ifdef _DEBUG
    //m_iCurModelIndex = 2;
    //m_iAnimIndex = 0;
    //m_isDebug = true;
    //m_fKeyFrame = 0.f;
    //m_vPos = _float3(42.f, 0.f, 42.f);
#endif // _DEBUG

    m_iNumMesh = m_pModelComs[m_iCurModelIndex]->Get_NumMeshes();
    m_vBaseColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_vClearColor = _float4(0.f, 0.f, 0.f, 0.f);
    m_iPassIndex = 1;

    m_pModelComs[m_iCurModelIndex]->Set_AnimationIndex(m_pParentTransformCom, m_iAnimIndex, true);

    CCamera* pCamera = m_pGameInstance->Find_Camera(TEXT("Camera_Clash"));

    if (nullptr == pCamera)
        return E_FAIL;
    m_pCameraTargetBoneMatrix = m_pModelComs[1]->Get_BoneMatrixPrt("b_cameratarget");
    pCamera->Set_CameraTargetBone(&m_CameraTargetBoneWorldMatrix);

    g_fTestDeltaTime = 1.f;




    return S_OK;
}

void CBody_Kamen::Priority_Update(_float fTimeDelta)
{
}

void CBody_Kamen::Update(_float fTimeDelta)
{
    //if(m_isDebug)
    //    m_isAnimationFinish = m_pModelComs[m_iCurModelIndex]->Play_Animation(fTimeDelta);
    //else
    //    m_isAnimationFinish = m_pModelComs[m_iCurModelIndex]->Play_Debug_Animation(m_fKeyFrame);

   m_isAnimationFinish = m_pModelComs[m_iCurModelIndex]->Play_Animation(fTimeDelta);

    /* 부모 행렬 적용 */
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));

    /* 카메라 타겟 설정 */
    XMStoreFloat4x4(&m_CameraTargetBoneWorldMatrix,
        XMLoadFloat4x4(m_pCameraTargetBoneMatrix) * XMLoadFloat4x4(&m_pParentTransformCom->Get_WorldMatrix()));

#ifdef _DEBUG
 /*   if (m_pModelComs[m_iCurModelIndex]->Get_LoopFlag())
    {
        m_EffectEvents.clear();
        const vector<EFFECT_EVENT_DESC>& EffectEvents = CGameManager::GetInstance()->Get_EffectTrack(CHARACTER::BOSS, m_iEffectID);

        m_EffectEvents.reserve(EffectEvents.size());

        for (const auto& Track : EffectEvents)
        {
            m_EffectEvents.push_back({ false, Track });
        }
    }

    for (auto& Event : m_EffectEvents)
    {
        if (false == Event.isTrigge)
        {
            if (m_pModelComs[m_iCurModelIndex]->Get_TrackPosition() >= Event.EventDesc.fKeyFrame)
            {
                Event.isTrigge = true;
                CGameManager::GetInstance()->Add_Effect(Event.EventDesc.eType, Event.EventDesc.iID, &m_pParentTransformCom->Get_WorldMatrix(), CHARACTER::BOSS);
            }
        }
    }*/
   
#endif // _DEBUG

}

void CBody_Kamen::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBody_Kamen::Render()
{
//#pragma region ANIMATION_TEST
//    ImGui::Begin("ANIM");
//    ImGui::Checkbox("Play", &m_isDebug);
//    ImGui::InputInt("EffectID", (_int*)(&m_iEffectID));
//    ImGui::SliderFloat("KeyFrmae", &m_fKeyFrame, 0.f, 300.f);
//    ImGui::InputFloat3("Pos", (_float*)(&m_vPos), "%.2f");
//    ImGui::InputInt("Animation", &m_iAnimIndex);
//    _int iIndex = {};
//    for (auto pName : m_pModelComs[m_iCurModelIndex]->Get_AnimationNames())
//    {
//        if (ImGui::Button(to_string(iIndex).c_str()))
//        {
//            m_iAnimIndex = iIndex;
//            m_pModelComs[m_iCurModelIndex]->Set_AnimationIndex(m_pParentTransformCom, m_iAnimIndex, true);
//            m_pParentTransformCom->Set_State(STATE::POSITION, XMVectorSet(45.f, 0.f, 45.f, 1.f));
//            m_EffectEvents.clear();
//            const vector<EFFECT_EVENT_DESC>& EffectEvents = CGameManager::GetInstance()->Get_EffectTrack(CHARACTER::BOSS, m_iEffectID);
//
//            m_EffectEvents.reserve(EffectEvents.size());
//
//            for (const auto& Track : EffectEvents)
//            {
//                m_EffectEvents.push_back({ false, Track });
//            }
//            
//        }
//        ++iIndex;
//        ImGui::SameLine();
//        ImGui::Text(pName);
//    }
//    ImGui::End();
//#pragma endregion
    m_iPassIndex = 1;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    if(2 != m_iCurModelIndex)
    {
        if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_RawValue("g_DiffuseColor", &m_vBaseColor, sizeof(_float4))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_Resource("g_EmissiveTexture", m_pEmssiveTextureCom->Get_SRV(0))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_EmissiveColor", &m_vBaseColor, sizeof(_float4))))
            return E_FAIL;
    }

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (2 == m_iCurModelIndex)
        {
            if (FAILED(m_pModelComs[m_iCurModelIndex]->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
                return E_FAIL;
            if (FAILED(m_pModelComs[m_iCurModelIndex]->Bind_Material(i, m_pShaderCom, "g_EmissiveTexture", TEXTURE::EMISSIVE)))
            {
                m_iPassIndex = 0;
                if (FAILED(m_pShaderCom->Bind_RawValue("g_EmissiveColor", &m_vClearColor, sizeof(_float4))))
                    return E_FAIL;
            }
            else
            {
                if (FAILED(m_pShaderCom->Bind_RawValue("g_EmissiveColor", &m_vBaseColor, sizeof(_float4))))
                    return E_FAIL;
            }
            
        }

        if (FAILED(m_pModelComs[m_iCurModelIndex]->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(1)))
            return E_FAIL;


        if (FAILED(m_pModelComs[m_iCurModelIndex]->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

void CBody_Kamen::Set_Animation(_uint iAnimationIdex, _bool bLoop, _float fLerpTime)
{
    m_isAnimationFinish = false;

    m_pModelComs[m_iCurModelIndex]->Set_AnimationIndex(m_pParentTransformCom, iAnimationIdex, bLoop, fLerpTime);
}

void CBody_Kamen::Change_Model(PHASE eType)
{
    if (PHASE::PHASE2 == eType)
    {
        m_iCurModelIndex = 1;
        m_pModelComs[m_iCurModelIndex]->Set_AnimationIndex(m_pParentTransformCom, 0, true);
        m_iNumMesh = m_pModelComs[m_iCurModelIndex]->Get_NumMeshes();
    }
    else if (PHASE::PHASE3 == eType)
    {
        m_iCurModelIndex = 2;
        m_pModelComs[m_iCurModelIndex]->Set_AnimationIndex(m_pParentTransformCom, 0, true);
        m_iNumMesh = m_pModelComs[m_iCurModelIndex]->Get_NumMeshes();
    }
}

HRESULT CBody_Kamen::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenBase"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenEmssive"),
        TEXT("Com_EmssiveTexture"), reinterpret_cast<CComponent**>(&m_pEmssiveTextureCom))))
        return E_FAIL;

    CModel* pModel = { nullptr };

    /* Com_Model */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&pModel))))
        return E_FAIL;

    m_pModelComs.push_back(pModel);

    /* Com_HorseModel */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_Horse"),
        TEXT("Com_HorseModel"), reinterpret_cast<CComponent**>(&pModel))))
        return E_FAIL;

    m_pModelComs.push_back(pModel);

    /* Com_Phase3Model */
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_3"),
        TEXT("Com_Phase3Model"), reinterpret_cast<CComponent**>(&pModel))))
        return E_FAIL;

    m_pModelComs.push_back(pModel);

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

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
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

    for(auto& pModel : m_pModelComs)
        Safe_Release(pModel);
    m_pModelComs.clear();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pEmssiveTextureCom);
}
