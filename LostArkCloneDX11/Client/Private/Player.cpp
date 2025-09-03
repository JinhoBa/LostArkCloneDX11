#include "pch.h"
#include "Player.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "PartObject.h"
#include "Body_Player.h"


CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject{ pDevice, pContext }, m_pGameManger{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameManger);
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject{ Prototype },
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

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(40.f, 0.f, 40.f, 1.f));

    m_PlayerInfo.eStance = STANCE::FLURRY;
    m_PlayerInfo.fHp = m_PlayerInfo.fMaxHp = 10000.f;
    m_PlayerInfo.fMp = m_PlayerInfo.fMaxMp = 8000.f;
    m_PlayerInfo.fIdentity = 0.f;
    m_PlayerInfo.fAttack = 10000.f;

    m_pBodyPlayer = dynamic_cast<CBody_Player*>(Find_PartObject(TEXT("Body_Player")));

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    if (MAX_IDENTITY >= m_PlayerInfo.fIdentity)
    {
        m_PlayerInfo.fIdentity += 2.f * fTimeDelta;
    }
    m_pGameManger->Update_Skills(fTimeDelta);

    Key_Input(fTimeDelta);

    if (m_pBodyPlayer->isAnimationFinish())
    {
        m_eCurState = IDLE;


    }

    if (MOVE == m_eCurState)
    {
        if (false == m_pTransformCom->MoveTo(fTimeDelta * 2.f, XMVectorSetW(XMLoadFloat3(m_pPickingPos), 1.f)))
        {
            m_eCurState = IDLE;
        }
    }

    Change_State();

    __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);

    __super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{

    return S_OK;
}
HRESULT CPlayer::Ready_PartObjects()
{
    CPartObject::PARTOBJECT_DESC Desc = {};
    Desc.pParentTransform = m_pTransformCom;

    if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"), TEXT("Body_Player"), &Desc)))
        return E_FAIL;

    return S_OK;
}
void CPlayer::Key_Input(_float fTimeDelta)
{
    if(STATE::IDLE == m_eCurState)
    {
        if (m_pGameInstance->Get_KeyDown(DIK_D))
        {
            if (m_pGameManger->Use_Skill(6))
            {
                m_eCurState = STATE::ATTACK;
                m_iSkillID = 30;
                m_bSkillLoop = false;
            }
        }
        if (m_pGameInstance->Get_KeyDown(DIK_E))
        {
            if (m_pGameManger->Use_Skill(2))
            {
                m_eCurState = STATE::ATTACK;
                m_iSkillID = 137;
                m_bSkillLoop = false;
            }
        }
        else if (m_pGameInstance->Get_KeyDown(DIK_Z))
        {
            if (m_PlayerInfo.fIdentity > 5.f)
            {
                Change_Stance();
            }
        }
        else if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
        {
            m_pPickingPos = m_pGameManger->Get_PickingPos();
            if(nullptr != m_pPickingPos)
                m_eCurState = STATE::MOVE;
        }
    }
    
}

void CPlayer::Change_Stance()
{
    m_PlayerInfo.eStance = STANCE::FLURRY == m_PlayerInfo.eStance ? STANCE::FOCUS : STANCE::FLURRY;

    if (40.f <= m_PlayerInfo.fIdentity)  //น๖วม
        m_PlayerInfo.fIdentity -= 40.f;
    else
        m_PlayerInfo.fIdentity = 0.f;
 
}
void  CPlayer::Change_State()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case Client::CPlayer::IDLE:
            m_iSkillID = STANCE::FLURRY == m_PlayerInfo.eStance ? 35 : 36;
            m_pBodyPlayer->Set_Animation(m_iSkillID, true);
            break;

        case Client::CPlayer::MOVE:
            m_iSkillID = STANCE::FLURRY == m_PlayerInfo.eStance ? 45 : 46;
            m_pBodyPlayer->Set_Animation(m_iSkillID, true);
            break;

        case Client::CPlayer::ATTACK:
            m_pBodyPlayer->Set_Animation(m_iSkillID);
            break;

        default:
            break;
        }
        m_ePreState = m_eCurState;
    }
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

    Safe_Release(m_pGameManger);
}
