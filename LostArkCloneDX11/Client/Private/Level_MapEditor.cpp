#include "pch.h"
#include "Level_MapEditor.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Camera_Free.h"
#include "MapObject.h"

CLevel_MapEditor::CLevel_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
    :CLevel{ pDevice, pContext, ENUM_TO_INT(eLevelID) },
    m_pGameManager{CGameManager::GetInstance()}
{
    Safe_AddRef(m_pGameManager);
}

HRESULT CLevel_MapEditor::Initialize()
{
    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Canvas(TEXT("Layer_Canvars"))))
        return E_FAIL;

    m_pBackGroundObject = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"));

    m_iComboIndex = 0;
    m_strSaveFileName;

    /*Texture*/
    m_pTextureCom = static_cast<CTextureMap*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Prototype_Component_TextureMap_MeshPreview")));

    if (nullptr == m_pTextureCom)
        return E_FAIL;
       
    m_pImagesNames = m_pGameManager->Get_PreviewTexturesPtr();
    if (nullptr == m_pImagesNames)
        return E_FAIL;


    m_PrototypeTags = { 
      /*  "Prototype_Component_Model_Kamen_chair",
        "Prototype_Component_Model_Kamen_chair01",
        "Prototype_Component_Model_Kamen_chair02",
        "Prototype_Component_Model_Kamen_floar01",
        "Prototype_Component_Model_Kamen_floar02",
        "Prototype_Component_Model_Kamen_floar03",
        "Prototype_Component_Model_Kamen_floar04",
        "Prototype_Component_Model_Kamen_floar05",
        "Prototype_Component_Model_Kamen_floar05a",
        "Prototype_Component_Model_Kamen_floar05b",
        "Prototype_Component_Model_Kamen_floar05c",
        "Prototype_Component_Model_Kamen_floar05d",
        "Prototype_Component_Model_Kamen_floar05e",
        "Prototype_Component_Model_Kamen_floar05f",
        "Prototype_Component_Model_Kamen_floar05g",
        "Prototype_Component_Model_Kamen_floar06",*/
        "Prototype_Component_Model_Trision",
        "Prototype_Component_Model_Trision2",
        "Prototype_Component_Model_Trision_Bottom",
        "Prototype_Component_Model_Trision_circle01",
        "Prototype_Component_Model_Trision_Object01",
        "Prototype_Component_Model_Trision_Stone01",
        "Prototype_Component_Model_Trision_Stone02",
        "Prototype_Component_Model_Trision_Stone03"
    };

    return S_OK;
}

void CLevel_MapEditor::Update(_float fTimeDelta)
{

}

HRESULT CLevel_MapEditor::Render()
{

    ImGui::Begin("Map Edit");

    ImGui::Text("Prototype : ");
    ImGui::Combo("1", &m_iComboIndex, m_PrototypeTags.data(), (int)m_PrototypeTags.size());

  /*  const _tchar* pFilePath = (*m_pImagesNames)[m_iComboIndex].c_str();
    ImGui::Image(ImTextureID(m_pTextureCom->Get_SRV(pFilePath)), ImVec2(128, 128));*/
    
    if (ImGui::Button("Add MapObject"))
    {
        Add_MapObject();
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
    
    

    ImGui::Begin("Object List");
    for (auto pObject : *m_pBackGroundObject)
    {
        CMapObject* pMapObject = dynamic_cast<CMapObject*>(pObject);

        if (nullptr == pMapObject)
            continue;
        if(ImGui::Button(m_pGameInstance->WstringToUtf8(pMapObject->Get_PrototypeTag()).c_str()))
        {
            m_pMapObject = pMapObject;
        }
    }
    ImGui::End();

    ImGui::Begin("EDIT");
    if (nullptr != m_pMapObject)
        m_pMapObject->Update_ImGui();
    ImGui::End();
   

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    ///* Background */
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"))))
        return E_FAIL;

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

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_Player(const _wstring& strLayerTag)
{
    // 0 : Player
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_Canvas(const _wstring& strLayerTag)
{
    // 0 : EXPBar
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
        return E_FAIL;

    // 1 : HUD
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
        return E_FAIL;

    // 2 : TopMenu
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TopMenu"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Add_MapObject()
{
    CMapObject::MAPOBJECT_DESC Desc = {};

    Desc.strPrototypeTag = m_pGameInstance->Utf8ToWstring(m_PrototypeTags[m_iComboIndex]);
    Desc.vPosition = _float3(0.f, 0.f, 0.f);
    Desc.vRotation = _float3(0.f, 0.f, 0.f);
    Desc.vScale = _float3(1.f, 1.f, 1.f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
        ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapEditor::Load_MapData()
{
    for (auto pObject : *m_pBackGroundObject)
    {
        CMapObject* pMapObject = dynamic_cast<CMapObject*>(pObject);

        if (nullptr == pMapObject)
            continue;
        pMapObject->Set_Dead();
    }

    vector<MAP_DATA>* pData = m_pGameManager->Get_MapDataPtr();

    for (auto MapData : *pData)
    {
        CMapObject::MAPOBJECT_DESC Desc = {};

        Desc.strPrototypeTag = MapData.strProtypeTag;
        Desc.vPosition = MapData.vPosition;
        Desc.vRotation = MapData.vRotation;
        Desc.vScale = MapData.vScale;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
            ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"), &Desc)))
            return E_FAIL;
    }
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

    Safe_Release(m_pGameManager);
    Safe_Release(m_pTextureCom);
}
