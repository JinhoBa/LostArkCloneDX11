#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }, m_pGameManger{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameManger);
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CGameObject{ Prototype }, 
    m_pGameManger{Prototype.m_pGameManger}
{
    Safe_AddRef(m_pGameManger);
}

HRESULT CPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    GAMEOBJECT_DESC Desc = {};
    Desc.fRotatePersec = 3.f;
    Desc.fSpeedPersec = 1.3f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(40.f, 0.f, 40.f, 1.f));

    m_iNumMesh = m_pModelCom->Get_NumMeshes();

    m_iAnimIndex = 35;
    m_bAnimLoop = true;
    m_bMove = true;


    m_Info.eStance = STANCE::FLURRY;
    m_Info.fHp = m_Info.fMaxHp = 10000.f;
    m_Info.fMp = m_Info.fMaxMp = 8000.f;
    m_Info.fIdentity = 0.f;
    m_Info.fAttack = 10000.f;

    m_pGameInstance->Add_Timer(TEXT("Timer_Stance"));
    m_pGameInstance->Compute_TimeDelta(TEXT("Timer_Stance"));

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    
}

void CPlayer::Update(_float fTimeDelta)
{
    if (MAX_IDENTITY >= m_Info.fIdentity)
    {
        m_Info.fIdentity += 2.f * fTimeDelta;
    }
    m_pGameManger->Update_Skills(fTimeDelta);


#pragma region TESTCODE
  
    //if (m_pGameInstance->Get_KeyPressing(DIK_SPACE))
    //{
    //    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), fTimeDelta);
    //}

    //if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::LBUTTON))
    //{
    //  
    //   //m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(CGameManager::GetInstance()->Get_PickingPos()), 1.f));
    //}

    _float3* pPickingPos = m_pGameManger->Get_PickingPos();
    if (nullptr != pPickingPos && m_bMove)
    {
        if (m_pTransformCom->MoveTo(fTimeDelta * 2.f, XMVectorSetW(XMLoadFloat3(pPickingPos), 1.f)))
        {
            m_bAnimLoop = true;
            m_iAnimIndex = 45;
        }
        else
        {
            m_bAnimLoop = true;
            m_iAnimIndex = 35;
        }
    }
#pragma endregion

    Key_Input(fTimeDelta);

    m_pModelCom->Play_Animation(m_iAnimIndex, fTimeDelta, m_bAnimLoop);
    if (m_pModelCom->IsAnimationFinished())
    {
        m_bAnimLoop = true;
        m_iAnimIndex = 35;
        m_bMove = true;
    }
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayer::Render()
{
#pragma region TESTCODE
    ImGui::InputInt("Animation", &m_iAnimIndex);
#pragma endregion

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
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

void CPlayer::Key_Input(_float fTimeDelta)
{
    if (m_pGameInstance->Get_KeyDown(DIK_D))
    {
        if(m_pGameManger->Use_Skill(6))
        {
            m_bAnimLoop = false;
            m_iAnimIndex = 30;
            m_bMove = false;
        }
    }
    else if (m_pGameInstance->Get_KeyDown(DIK_Z))
    {
        if (m_Info.fIdentity > 5.f)
        {
            Change_Stance();      
        }
    }

}

HRESULT CPlayer::Add_Components()
{
    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /*Shader_VTXAnimTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

void CPlayer::Change_Stance()
{
    m_Info.eStance = STANCE::FLURRY == m_Info.eStance ? STANCE::FOCUS : STANCE::FLURRY;

    if (40.f <= m_Info.fIdentity)  //น๖วม
        m_Info.fIdentity -= 40.f;
    else
        m_Info.fIdentity = 0.f;
 
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

    Safe_Release(m_pGameManger);
}
