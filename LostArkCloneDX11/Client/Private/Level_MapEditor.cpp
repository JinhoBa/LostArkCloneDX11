#include "pch.h"
#include "Level_MapEditor.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Camera_Free.h"
#include "MapObject.h"
#include "Terrain.h"
#include "Navigation_Tool.h"

CLevel_MapEditor::CLevel_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) },
    m_pGameManager{CGameManager::GetInstance()}
{
    Safe_AddRef(m_pGameManager);
}

HRESULT CLevel_MapEditor::Initialize()
{
    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Canvas(TEXT("Layer_Canvars"))))
        return E_FAIL;

    //m_pBackGroundObject = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"));
    //m_pTerrains = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"));

    m_iMapObject_ComboIndex = 0;
    m_iTerrain_ComboIndex = 0;
    m_strSaveFileName;

    m_iPointIndex = 0;

    for (_uint i = 0; i < 3; i++)
    {
        m_bClicked[i] = false;
        m_Points[i] = _float3(0.f, 0.f, 0.f);
    }

    m_MapObject_PrototypeTags = { 
        // Kamen
       "Prototype_Component_Model_Kamen_chair01",
        "Prototype_Component_Model_Kamen_chair01a",
        "Prototype_Component_Model_Kamen_chair01b",
        "Prototype_Component_Model_Kamen_floar01",
        "Prototype_Component_Model_Kamen_floar02",
        "Prototype_Component_Model_Kamen_floar03",
        "Prototype_Component_Model_Kamen_floar04",
       // "Prototype_Component_Model_Kamen_floar05",
        "Prototype_Component_Model_Kamen_floar05a",
        "Prototype_Component_Model_Kamen_floar05b",
        "Prototype_Component_Model_Kamen_floar05c",
        "Prototype_Component_Model_Kamen_floar05d",
        "Prototype_Component_Model_Kamen_floar05e",
        "Prototype_Component_Model_Kamen_floar05f",
        "Prototype_Component_Model_Kamen_floar05g",
        "Prototype_Component_Model_Kamen_floar05h",
        "Prototype_Component_Model_Kamen_floar06",
        "Prototype_Component_Model_Kamen_floar07",
        "Prototype_Component_Model_Kamen_floar08",
        "Prototype_Component_Model_Kamen_floar08b",
        "Prototype_Component_Model_Kamen_floar08e",
        "Prototype_Component_Model_Kamen_floar09",
        "Prototype_Component_Model_Kamen_floar10",
        "Prototype_Component_Model_Kamen_floar11",
        "Prototype_Component_Model_Kamen_floar11a",
        "Prototype_Component_Model_Kamen_floar12",
        "Prototype_Component_Model_Kamen_floar12a",
        "Prototype_Component_Model_Kamen_deco01",
        "Prototype_Component_Model_Kamen_deco02",
        "Prototype_Component_Model_Kamen_deco02c",
        "Prototype_Component_Model_Kamen_deco02a",
        "Prototype_Component_Model_Kamen_deco02b",
        "Prototype_Component_Model_Kamen_deco03",
        "Prototype_Component_Model_Kamen_deco04",
       // "Prototype_Component_Model_Kamen_deco05",
        "Prototype_Component_Model_Kamen_deco06",
        "Prototype_Component_Model_Kamen_pillar01",
        "Prototype_Component_Model_Kamen_pillar01a",
        "Prototype_Component_Model_Kamen_pillar02",
        //"Prototype_Component_Model_Kamen_pillar02a",
        "Prototype_Component_Model_Kamen_pillar02low",
        "Prototype_Component_Model_Kamen_pillar03",
        "Prototype_Component_Model_Kamen_pillar04",
        "Prototype_Component_Model_Kamen_pillar05",
        "Prototype_Component_Model_Kamen_pillar06",
        "Prototype_Component_Model_Kamen_pillar07",
        "Prototype_Component_Model_Kamen_pillar08",
        "Prototype_Component_Model_Kamen_pillar09",
        "Prototype_Component_Model_Kamen_pillar09a",
        "Prototype_Component_Model_Kamen_pillar09low",
        "Prototype_Component_Model_Kamen_gate01",

        // Trision
     /*   "Prototype_Component_Model_Trision",
        "Prototype_Component_Model_Trision2",
        "Prototype_Component_Model_Trision01b",
        "Prototype_Component_Model_Trision_Botton01",
        "Prototype_Component_Model_Trision_Botton01a",
        "Prototype_Component_Model_Trision_Botton02",
        "Prototype_Component_Model_Trision_Botton03",
        "Prototype_Component_Model_Trision_circle01",
        "Prototype_Component_Model_Trision_Object01",
        "Prototype_Component_Model_Trision_Stone01",
        "Prototype_Component_Model_Trision_Stone02",
        "Prototype_Component_Model_Trision_Stone03",
        "Prototype_Component_Model_Trision_2c",
        "Prototype_Component_Model_Trision_Stair01a",
        "Prototype_Component_Model_Trision_Stair01b",
        "Prototype_Component_Model_Trision_Stair01c",
        "Prototype_Component_Model_Trision_Stair01d",
        "Prototype_Component_Model_Trision_Stair01e",
        "Prototype_Component_Model_Trision_Stair01f",
        "Prototype_Component_Model_Trision_Wall",
        "Prototype_Component_Model_Trision_arkprop"*/

    };

    m_Terrain_PrototypeTags = {
        "Prototype_Component_VIBuffer_Terrain_Trision_Floor",
        "Prototype_Component_VIBuffer_Terrain_Trision_Stair",
        "Prototype_Component_VIBuffer_Terrain_80",
        "Prototype_Component_VIBuffer_Terrain_64",
        "Prototype_Component_VIBuffer_Terrain_45",
        "Prototype_Component_VIBuffer_Terrain_32",
        "Prototype_Component_VIBuffer_Terrain_16"
    };

    m_pNavigation_Tool = CNavigation_Tool::Create(m_pDevice, m_pContext);
 

    return S_OK;
}

void CLevel_MapEditor::Update(_float fTimeDelta)
{ 

#pragma region NAVI

    /* 새로운 점 선택 */
 if(m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
 {
     if(3 > m_iPointIndex)
     {
         XMStoreFloat3(&m_vPickingPositon, m_pGameManager->Picking_Terrains());

         if(0.f < m_vPickingPositon.x)
         {
             memcpy(&m_Points[m_iPointIndex], &m_vPickingPositon, sizeof(_float3));
             m_bClicked[m_iPointIndex] = true;
             m_iPointIndex++;
         }
     }
 }

 /* 기존 점 선택 */
 if (m_pGameInstance->Get_KeyPressing(DIK_LCONTROL) && m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::LBUTTON))
 {
     if (3 > m_iPointIndex)
     {
         XMStoreFloat3(&m_vPickingPositon, m_pGameManager->Picking_Terrains());

         if (true == m_pNavigation_Tool->Pick_Cell(XMLoadFloat3(&m_vPickingPositon), &m_vPickingPositon))
         {
             memcpy(&m_Points[m_iPointIndex], &m_vPickingPositon, sizeof(_float3));
             m_bClicked[m_iPointIndex] = true;
             m_iPointIndex++;
         }
     }
 }

 /* 선택 취소 */
 if (m_pGameInstance->Get_KeyDown(DIK_Z) && m_pGameInstance->Get_KeyPressing(DIK_LCONTROL))
 {
     if (0 < m_iPointIndex && 4 > m_iPointIndex)
     {
         --m_iPointIndex;
         m_bClicked[m_iPointIndex] = false;
         m_Points[m_iPointIndex] = _float3(0.f, 0.f, 0.f);
     }
 }

 /* Cell 생성 */

 if (m_bClicked[0] && m_bClicked[1] && m_bClicked[2])
 {
     if (FAILED(m_pNavigation_Tool->Add_Sell(m_Points)))
         return;
     else
     {
         for (_uint i = 0; i < 3; i++)
         {
             m_bClicked[i] = false;
             m_Points[i] = _float3(0.f, 0.f, 0.f);
             m_iPointIndex = 0;
         }
     }
 }


 /* Cell 삭제 */
 if (m_pGameInstance->Get_KeyDown(DIK_BACKSPACE))
 {
     m_pNavigation_Tool->Remove_Sell();
 }
 
#pragma endregion
 
}

HRESULT CLevel_MapEditor::Render()
{
#pragma region NAVI
    ImGui::Begin("NAVIGATION");

     ImGui::Text("CELL POINTS  :");
     ImGui::SliderFloat3 ("A", reinterpret_cast<_float*>(&m_Points[0]), 0.1f, 1.f);
     ImGui::SliderFloat3("B", reinterpret_cast<_float*>(&m_Points[1]), 0.1f, 1.f);
     ImGui::SliderFloat3("C", reinterpret_cast<_float*>(&m_Points[2]), 0.1f, 1.f);

     if (ImGui::Button("Save Navigation File"))
     {
         if (FAILED(m_pNavigation_Tool->Save_File("../Bin/Resources/Data/Navigtion/Test.bin")))
             MSG_BOX("Success Save File");
         else
             MSG_BOX("Failed to Save File");
     }

     if (ImGui::Button("Load Navigation File"))
     {
         if(FAILED(m_pNavigation_Tool->LoadFile("../Bin/Resources/Data/Navigtion/Kamen2_Navigation.bin")))
             MSG_BOX("Failed to Load File");
         else
         {
             MSG_BOX("Successed to Load File!");
         }


     }
     if (ImGui::Button("Load"))
     {

     }

     ImGui::End();
#pragma endregion

 


#pragma region MAP_EDIT
    ImGui::Begin("Map Edit");

    ImGui::Text("Prototype MapObject : ");
    ImGui::Combo("MapObject", &m_iMapObject_ComboIndex, m_MapObject_PrototypeTags.data(), (int)m_MapObject_PrototypeTags.size());


    if (ImGui::Button("Add MapObject"))
    {
        Add_MapObject();
    }

    ImGui::Text("Prototype Terrain : ");
    ImGui::Combo("Terrain", &m_iTerrain_ComboIndex, m_Terrain_PrototypeTags.data(), (int)m_Terrain_PrototypeTags.size());
    if (ImGui::Button("Add Terrain"))
    {
        if (FAILED(Add_Terrain()))
        {
            ImGui::End();
            return E_FAIL;
        }

    }

    if (ImGui::Button("Load"))
    {
        if (FAILED(Load_MapData()))
        {
            ImGui::End();
            return E_FAIL;
        }
        SetWindowText(g_hWnd, TEXT("Load MapData"));
    }
    ImGui::Text("-------------------");
    ImGui::Text("Save File Name : ");
    ImGui::InputText("##name", &m_strSaveFileName[0], MAX_PATH);
    if (ImGui::Button("Save"))
    {
        m_pGameManager->Save_MapData(m_strSaveFileName.c_str());
        SetWindowText(g_hWnd, TEXT("Save MapData"));
    }

    ImGui::End();
#pragma endregion

    

#pragma region TERRAIN_LIST

       ImGui::Begin("Terrain List");
       _uint iTerrainIndex = {};
       string strTmp;
       const list<CGameObject*>& Terrain = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"));
       if(!Terrain.empty())
       {

           for (auto& pObject : Terrain)
           {
               CTerrain* pTerrain = dynamic_cast<CTerrain*>(pObject);

               if (nullptr == pTerrain)
                   continue;

               strTmp = "Terrain##" + to_string(iTerrainIndex++);

               if (ImGui::Button(strTmp.c_str()))
               {
                   m_pTerrain = pTerrain;
               }
           }
       }
       ImGui::End();
#pragma endregion


#pragma region OBJECT_LIST
       
    ImGui::Begin("Object List");
    _uint iObjectIndex = {};
    string strSrc;

    const list<CGameObject*>& BackGroundObject = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"));
    for (auto pObject : BackGroundObject)
    {
        CMapObject* pMapObject = dynamic_cast<CMapObject*>(pObject);

        if (nullptr == pMapObject)
            continue;

        strSrc = m_pGameInstance->WstringToUtf8(pMapObject->Get_PrototypeTag()) + to_string(iObjectIndex) + "##" + to_string(iObjectIndex++);

        if (ImGui::Button(strSrc.c_str()))
        {
            m_pMapObject = pMapObject;
        }
    }
    ImGui::End();

    ImGui::Begin("EDIT");
    if (ImGui::CollapsingHeader("Terrain"))
    {
        if (nullptr != m_pTerrain)
            m_pTerrain->Update_ImGui();
    }
    if (ImGui::CollapsingHeader("Object"))
    {
        if (nullptr != m_pMapObject)
            m_pMapObject->Update_ImGui();
    }
    ImGui::End();
    
#pragma endregion


    m_pNavigation_Tool->Render();

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Light()
{
    LIGHT_DESC Desc = {};

    Desc.eType = LIGHT::DIRECTIONAL;
    Desc.vDiffuse = _float4(7.f, 7.f, 7.f, 1.f);
    Desc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
    Desc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 0.5f);
    Desc.vDirection = _float4(0.5f, 0.5f, 0.5f, 0.f);

    if (FAILED(m_pGameInstance->Add_Light(L"Test", Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    CCamera::CAMERA_DESC Desc = {};

    Desc.fNear = 0.1f;
    Desc.fFar = 500.f;
    Desc.fFovy = XMConvertToRadians(60.f);
    Desc.vEye = _float3(0.f, 5.f, -5.f);
    Desc.vLookAt = _float3(0.f, 0.f, 0.f);
    Desc.fSpeedPersec = 5.f;
    Desc.fRotatePersec = XMConvertToRadians(90.f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag, &Desc)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Prototype_GameObject_SkySphere"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Sky"), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_Player(const _wstring& strLayerTag)
{
    //// 0 : Player
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
    //    ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
    //    return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_Canvas(const _wstring& strLayerTag)
{
    //// 0 : EXPBar
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
    //    ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
    //    return E_FAIL;

    //// 1 : HUD
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
    //    ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
    //    return E_FAIL;

    //// 2 : TopMenu
    //if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TopMenu"),
    //    ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
    //    return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Add_MapObject()
{
    CMapObject::MAPOBJECT_DESC Desc = {};
    Desc.iLevelIndex = ENUM_TO_INT(LEVEL::GAMEPLAY);
    Desc.strPrototypeTag = m_pGameInstance->Utf8ToWstring(m_MapObject_PrototypeTags[m_iMapObject_ComboIndex]);
    Desc.vPosition = _float3(0.f, 0.f, 0.f);
    Desc.vRotation = _float3(0.f, 0.f, 0.f);
    Desc.vScale = _float3(2.5f, 2.5f, 2.5f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"), &Desc)))
        return E_FAIL;

    /*CMapObject* pMapObject = dynamic_cast<CMapObject*>(m_pBackGroundObject->back());
    if (nullptr == pMapObject)
        return S_OK;

    m_pMapObject = pMapObject;*/

    return S_OK;
}

HRESULT CLevel_MapEditor::Add_Terrain()
{
    CTerrain::TERRAIN_DESC Desc = {};

    Desc.fRotatePersec = 0.f;
    Desc.fSpeedPersec = 0.f;
    Desc.strPrototypeTag = m_pGameInstance->Utf8ToWstring(m_Terrain_PrototypeTags[m_iTerrain_ComboIndex]);
    Desc.vPosition = _float3(0.f, 0.f, 0.f);
    Desc.vRotation = _float3(0.f, 0.f, 0.f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Load_MapData()
{

    for (const auto& TerrainData : m_pGameManager->Get_TerrainData())
    {
        CTerrain::TERRAIN_DESC Desc = {};

        Desc.strPrototypeTag = TerrainData.strPrototypeTag;
        Desc.vPosition = TerrainData.vPosition;
        Desc.vRotation = TerrainData.vRotation;
 

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
            ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"), &Desc)))
            return E_FAIL;
    }

    for (const auto& MapData : m_pGameManager->Get_MapData())
    {
        CMapObject::MAPOBJECT_DESC Desc = {};

        Desc.iLevelIndex = ENUM_TO_INT(LEVEL::GAMEPLAY);
        Desc.strPrototypeTag = MapData.strPrototypeTag;
        Desc.vPosition = MapData.vPosition;
        Desc.vRotation = MapData.vRotation;
        Desc.vScale = MapData.vScale;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
            ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CLevel_MapEditor::Save_MapData()
{
    return S_OK;
}

CLevel_MapEditor* CLevel_MapEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
    CLevel_MapEditor* pInstance = new CLevel_MapEditor(pDevice, pContext, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Create : CLevel_MapEditor");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLevel_MapEditor::Free()
{
    __super::Free();

    Safe_Release(m_pNavigation_Tool);

    Safe_Release(m_pGameManager);
}
