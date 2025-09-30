#include "pch.h"
#include "Camera_Npc.h"

#include "GameInstance.h"

CCamera_Npc::CCamera_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{ pDevice, pContext }
{
}

CCamera_Npc::CCamera_Npc(const CCamera_Npc& Prototype)
    :CCamera{ Prototype }
{
}

HRESULT CCamera_Npc::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Npc::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_vTargetPosition = _float3(40.f, 0.f, 39.f);
    m_vDirection = _float3(-2.7f, 2.3f, 3.6f);

    return S_OK;
}

void CCamera_Npc::Priority_Update(_float fTimeDelta)
{
    Update_Camera_Position();

    __super::Bind_Transform();
}

void CCamera_Npc::Update(_float fTimeDelta)
{
}

void CCamera_Npc::Late_Update(_float fTimeDelta)
{
#ifdef _DEBUG
    //m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
#endif // _DEBUG

}

HRESULT CCamera_Npc::Render()
{
    ImGui::SliderFloat3("Target", reinterpret_cast<_float*>(&m_vTargetPosition), 0.f, 50.f);
    ImGui::SliderFloat3("Dircetion", reinterpret_cast<_float*>(&m_vDirection), -10.f, 10.f);

    return S_OK;
}

void CCamera_Npc::Reset()
{
    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));
}

void CCamera_Npc::Update_Camera_Position()
{
    m_pTransformCom->Set_State(STATE::POSITION,
        XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f) + XMLoadFloat3(&m_vDirection));

    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f));
}


CCamera_Npc* CCamera_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCamera_Npc* pInstance = new CCamera_Npc(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CCamera_Npc");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Npc::Clone(void* pArg)
{
    CGameObject* pInstance = new CCamera_Npc(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CCamera_Npc");
        return nullptr;
    }

    return pInstance;
}

void CCamera_Npc::Free()
{
    __super::Free();
}
