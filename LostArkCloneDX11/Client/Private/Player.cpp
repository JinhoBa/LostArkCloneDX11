#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 5.f;
    Desc.fSpeedPersec = 5.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
    m_pTransformCom->Rotation(m_pTransformCom->Get_State(STATE::UP), XMConvertToRadians(180.f));

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    
}

void CPlayer::Update(_float fTimeDelta)
{
#pragma region TESTCODE
    //if (m_pGameInstance->Get_KeyPressing(DIK_W))
    //{
    //    m_pTransformCom->Go_Straight(fTimeDelta);
    //}
    //if (m_pGameInstance->Get_KeyPressing(DIK_S))
    //{
    //    m_pTransformCom->Go_Backward(fTimeDelta);
    //}
    //if (m_pGameInstance->Get_KeyPressing(DIK_A))
    //{
    //    m_pTransformCom->Go_Left(fTimeDelta);
    //}
    //if (m_pGameInstance->Get_KeyPressing(DIK_D))
    //{
    //    m_pTransformCom->Go_Right(fTimeDelta);
    //}
    //if (m_pGameInstance->Get_KeyPressing(DIK_SPACE))
    //{
    //    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), fTimeDelta);
    //}

    //if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::LBUTTON))
    //{
    //  
    //   //m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(CGameManager::GetInstance()->Get_PickingPos()), 1.f));
    //}
    _float3* pPickingPos = CGameManager::GetInstance()->Get_PickingPos();
    if(nullptr != pPickingPos)
        m_pTransformCom->MoveTo(fTimeDelta* 2.f, XMVectorSetW(XMLoadFloat3(pPickingPos), 1.f));


#pragma endregion

}

void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayer::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

   

    return S_OK;
}

HRESULT CPlayer::Add_Components()
{
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

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CPlayer");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CGameObject* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CPlayer");
        return nullptr;
    }

    return pInstance;
}

void CPlayer::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
