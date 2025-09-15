#include "pch.h"
#include "Level_Boss.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Camera_Fix.h"
#include "Terrain.h"
#include "MapObject.h"
#include "Monster.h"
#include "Player.h"

CLevel_Boss::CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) }
{
}

HRESULT CLevel_Boss::Initialize()
{
    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Camera()))
        return E_FAIL;

    if (FAILED(Ready_Player()))
        return E_FAIL;

    if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Kamen(TEXT("Layer_Kamen"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Boss::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Boss::Render()
{

    return S_OK;
}

HRESULT CLevel_Boss::Ready_Light()
{
    LIGHT_DESC Desc = {};

    Desc.eType = LIGHT::DIRECTIONAL;
    Desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    Desc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    Desc.vSpecular = _float4(0.01f, 0.01f, 0.01f, 0.1f);
    Desc.vDirection = _float4(0.5f, 0.5f, -0.5f, 0.f);

    if (FAILED(m_pGameInstance->Add_Light(Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Boss::Ready_Camera()
{
    dynamic_cast<CCamera_Fix*>(
        m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Camera")).back())
        ->Set_LookDircetion(XMVectorSet(0.f, 5.f, -5.f, 0.f));

    return S_OK;
}

HRESULT CLevel_Boss::Ready_Player()
{
   if(FAILED(dynamic_cast<CPlayer*>(
        m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())
        ->Change_Level(XMVectorSet(38.f, 14.f, 8.f, 1.f), TEXT("Prototype_Component_Navigation_Kamen"))))
       return E_FAIL;


    return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    /* Background */
    if (FAILED(Load_MapData()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Kamen(const _wstring& strLayerTag)
{
    ///* Kamen */
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Kamen"),
    //    ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
    //    return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Kamen_Sword"),
       ENUM_TO_INT(LEVEL::BOSS), strLayerTag)))
       return E_FAIL;


    //CMonster::MONSTER_DESC Desc = {};

    //Desc.iMonsterID = 0;
    //Desc.iNumAttack = 4;
    //Desc.fAttack = 500.f;
    //Desc.fAttackRange = 1.5f;
    //Desc.fDetectDistance = 3.f;
    //Desc.fMaxHp = Desc.fHp = 10000.f;
    //Desc.fSpeedPersec = 3.f;
    //Desc.fRotatePersec = 5.f;
    //Desc.vPosition = _float4(50.f, 0.f, 50.f, 1.f);
    //Desc.strModelPrototypeTag = L"Prototype_Component_Model_Monster1";

    //// 0 : Monter1
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Named"),
    //    ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
    //    return E_FAIL;

    //Desc.iMonsterID = 1;
    //Desc.iNumAttack = 2;
    //Desc.fMaxHp = Desc.fHp = 5000.f;
    //Desc.fAttackRange = 2.f;
    //Desc.strModelPrototypeTag = L"Prototype_Component_Model_Monster2";

    //for (_uint i = 0; i < 20; i++)
    //{
    //    Desc.vPosition = _float4(40.f + m_pGameInstance->Random(-5.f, 5.f), 0.f, 40.f + m_pGameInstance->Random(-5.f, 5.f), 1.f);
    //    // 0 : Monter2
    //    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Named"),
    //        ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
    //        return E_FAIL;
    //}


    return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_SkyBox(const _wstring& strLayerTag)
{
   if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Dynamic_SkyBox"),
        ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Sky"))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Boss::Load_MapData()
{
    for (const auto& TerrainData : CGameManager::GetInstance()->Get_TerrainData())
    {
        CTerrain::TERRAIN_DESC Desc = {};

        Desc.strPrototypeTag = TerrainData.strPrototypeTag;
        Desc.vPosition = TerrainData.vPosition;
        Desc.vRotation = TerrainData.vRotation;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
            ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Terrain"), &Desc)))
            return E_FAIL;
    }


    for (auto& MapData : CGameManager::GetInstance()->Get_MapData())
    {
        CMapObject::MAPOBJECT_DESC Desc = {};

        Desc.iLevelIndex = ENUM_TO_INT(LEVEL::BOSS);
        Desc.strPrototypeTag = MapData.strPrototypeTag;
        Desc.vPosition = MapData.vPosition;
        Desc.vRotation = MapData.vRotation;
        Desc.vScale = MapData.vScale;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
            ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_Background"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}


CLevel_Boss* CLevel_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
    CLevel_Boss* pInstance = new CLevel_Boss(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CLevel_Boss");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_Boss::Free()
{
    __super::Free();
}
