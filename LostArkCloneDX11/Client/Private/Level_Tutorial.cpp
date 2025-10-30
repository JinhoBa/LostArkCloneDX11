#include "pch.h"
#include "Level_Tutorial.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Level_Loading.h"

#include "Camera_Free.h"
#include "Camera_Fix.h"
#include "Camera_ChargeSkill.h"
#include "Terrain.h"
#include "MapObject.h"
#include "Monster.h"

CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{pDevice, pContext, ENUM_TO_INT(eLevelID)}
{
}

HRESULT CLevel_Tutorial::Initialize()
{
    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Camera()))
        return E_FAIL;

    if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
       return E_FAIL;

    //if (FAILED(Ready_Layer_Kamen(TEXT("Layer_Kamen"))))
    //    return E_FAIL;

   /* if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Npc(TEXT("Layer_Npc"))))
        return E_FAIL;*/

    if (FAILED(Ready_Layer_Canvas(TEXT("Layer_Canvars"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect_Manager"))))
        return E_FAIL;

    m_pGameInstance->PlayBGM(L"Trision_BGM.wav", 0.1f);
    //m_pGameInstance->Bind_Camera(TEXT("Camera_Free"));

    return S_OK;
}

void CLevel_Tutorial::Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_KeyDown(DIK_F1))
    {
        m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOADING, LEVEL::BOSS));
    }
}

HRESULT CLevel_Tutorial::Render()
{

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Light()
{
    LIGHT_DESC Desc = {};

    Desc.eType = LIGHT::DIRECTIONAL;
    Desc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
    Desc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
    Desc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);
    Desc.vDirection = _float4(0.f, -1.f, -0.1f, 0.f);

    if (FAILED(m_pGameInstance->Add_Light(L"Dircection", Desc)))
        return E_FAIL;

    Desc.eType = LIGHT::POINT;
    Desc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
    Desc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
    Desc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);
    Desc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);

    if (FAILED(m_pGameInstance->Add_Light(L"Point1", Desc)))
        return E_FAIL;


    SHADOW_LIGHT_DESC ShadowDesc = {};

    ShadowDesc.vEye = _float4(40.f, 30.f, 50.f, 1.f);
    ShadowDesc.vAt = _float4(40.f, 0.f, 40.f, 1.f);
    ShadowDesc.fFovy = XMConvertToRadians(80.f);
    ShadowDesc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
    ShadowDesc.fNear = 0.1f;
    ShadowDesc.fFar = 500.f;

    if (FAILED(m_pGameInstance->Ready_Shadow_Light(ShadowDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    /* Background */
    if (FAILED(Load_MapData()))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_SkySphere"),
        ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Layer_Sky"))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Player(const _wstring& strLayerTag)
{
    // 0 : Player
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;



    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Kamen(const _wstring& strLayerTag)
{
    /* Kamen */
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Kamen"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Monster(const _wstring& strLayerTag)
{
    CMonster::MONSTER_DESC Desc = {};

    Desc.iMonsterID = 0;
    Desc.iNumAttack = 4;
    Desc.fAttack = 500.f;
    Desc.fAttackRange = 1.5f;
    Desc.fDetectDistance = 3.f;
    Desc.fMaxHp = Desc.fHp = 100000.f;
    Desc.fSpeedPersec = 3.f;
    Desc.fRotatePersec = 5.f;
    Desc.vPosition = _float4(40.f, 0.f, 38.f, 1.f);
    Desc.strModelPrototypeTag = L"Prototype_Component_Model_Monster1";

    ////// 0 : Monter1
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Named"),
        ENUM_TO_INT(LEVEL::TUTORIAL), strLayerTag, &Desc)))
        return E_FAIL;

    Desc.iMonsterID = 1;
    Desc.iNumAttack = 2;
    Desc.fMaxHp = Desc.fHp = 50000.f;
    Desc.fAttackRange = 2.f;
    Desc.strModelPrototypeTag = L"Prototype_Component_Model_Monster2";

    
    // 0 : Monter2
    for (size_t i = 0; i < 5; i++)
    {
        Desc.vPosition = _float4(40.f + m_pGameInstance->Random(-5.f, 5.f), 0.f, 40.f + m_pGameInstance->Random(-5.f, 5.f), 1.f);
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Named"),
            ENUM_TO_INT(LEVEL::TUTORIAL), strLayerTag, &Desc)))
            return E_FAIL;
    }
  

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Npc(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_Npc"),
        ENUM_TO_INT(LEVEL::TUTORIAL), strLayerTag, nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Camera()
{
    CCamera::CAMERA_DESC Desc = {};

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);
    m_pGameInstance->Add_Camera(TEXT("Camera_Fix"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Fix"), &Desc)));

    if (FAILED(m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"))))
        return E_FAIL;

    m_pGameInstance->Find_Camera(TEXT("Camera_Fix"))->Set_LookDircetion(XMVectorSet(0.f, 5.f, 5.f, 0.f));

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);

    m_pGameInstance->Add_Camera(TEXT("Camera_ChargeSkill"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_ChargeSkill"), &Desc)));

    m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_LookDircetion(XMVectorSet(0.f, 5.f, 5.f, 0.f));

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(40.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 3.2f, -13.3f);
    if (FAILED(m_pGameInstance->Add_Camera(TEXT("Camera_Kamen_Intro"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Kamen_Intro"), &Desc)))))
        return E_FAIL;

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);
    if (FAILED(m_pGameInstance->Add_Camera(TEXT("Camera_Free"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"), &Desc)))))
        return E_FAIL;

    //if (FAILED(m_pGameInstance->Bind_Camera(TEXT("Camera_Free"))))
    //    return E_FAIL;

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);
    if (FAILED(m_pGameInstance->Add_Camera(TEXT("Camera_Enter"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_KamenEnter"), &Desc)))))
        return E_FAIL;

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(40.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);
    if (FAILED(m_pGameInstance->Add_Camera(TEXT("Camera_Clash"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Clash"), &Desc)))))
        return E_FAIL;

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);
    Desc.vDirection = _float3(0.f, 5.f, -5.f);
    if (FAILED(m_pGameInstance->Add_Camera(TEXT("Camera_Npc"), dynamic_cast<CCamera*>(m_pGameInstance->Clone_Prototype(
        PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Npc"), &Desc)))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Canvas(const _wstring& strLayerTag)
{
    // 0 : EXPBar
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    // 1 : HUD
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    // 2 : TopMenu
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TopMenu"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    // 3 : HoldingSkillUI
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HoldingSkillUI"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    // Screen Effect
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CScreen_Effect"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Screen_Effect"))))
        return E_FAIL;

    return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_Effect(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Manager"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    CGameManager::GetInstance()->Initialize_Effect_Manager();

    return S_OK;
}
HRESULT CLevel_Tutorial::Load_MapData()
{
    for (const auto& TerrainData : CGameManager::GetInstance()->Get_TerrainData())
    {
        CTerrain::TERRAIN_DESC Desc = {};

        Desc.strPrototypeTag = TerrainData.strPrototypeTag;
        Desc.vPosition = TerrainData.vPosition;
        Desc.vRotation = TerrainData.vRotation;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
            ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Layer_Terrain"), &Desc)))
            return E_FAIL;
    }


    for (auto& MapData : CGameManager::GetInstance()->Get_MapData())
    {
        CMapObject::MAPOBJECT_DESC Desc = {};

        Desc.iLevelIndex = ENUM_TO_INT(LEVEL::TUTORIAL);
        Desc.strPrototypeTag = MapData.strPrototypeTag;
        Desc.vPosition = MapData.vPosition;
        Desc.vRotation = MapData.vRotation;
        Desc.vScale = MapData.vScale;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
            ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Layer_Background"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}


CLevel_Tutorial* CLevel_Tutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
    CLevel_Tutorial* pInstance = new CLevel_Tutorial(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CLevel_Tutorial");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_Tutorial::Free()
{
    __super::Free();
}
