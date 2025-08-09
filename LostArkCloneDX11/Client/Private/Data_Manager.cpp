#include "pch.h"
#include "Data_Manager.h"

#include "GameInstance.h"
#include "MapObject.h"

CData_Manager::CData_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    m_MapDatas.reserve(100);
}

HRESULT CData_Manager::Load_File(const _char* pFilePath)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Land");

    if (nullptr == root)
    {
        MSG_BOX("Failed to Find root");
        return E_FAIL;
    }
    MAP_DATA Map_Data = {};

    for (auto* land = root->FirstChildElement("Model"); land; land = land->NextSiblingElement("Model"))
    {
        tinyxml2::XMLElement* protoElem = land->FirstChildElement("PrototypeTag");

        const _char* txt = protoElem->GetText();

        if (nullptr != txt)
        {
            Map_Data.strProtypeTag = CGameInstance::GetInstance()->Utf8ToWstring(txt);
        }
        else
        {
            Map_Data.strProtypeTag = L"";
        }
        
        
        protoElem = land->FirstChildElement("Position");
        if (nullptr == protoElem)
            return E_FAIL;
        protoElem->QueryFloatAttribute("x", &Map_Data.vPosition.x);
        protoElem->QueryFloatAttribute("y", &Map_Data.vPosition.y);
        protoElem->QueryFloatAttribute("z", &Map_Data.vPosition.z);

        protoElem = land->FirstChildElement("Scale");
        if (nullptr == protoElem)
            return E_FAIL;
        protoElem->QueryFloatAttribute("x", &Map_Data.vScale.x);
        protoElem->QueryFloatAttribute("y", &Map_Data.vScale.y);
        protoElem->QueryFloatAttribute("z", &Map_Data.vScale.z);

        protoElem = land->FirstChildElement("Rotation");
        if (nullptr == protoElem)
            return E_FAIL;
        protoElem->QueryFloatAttribute("x", &Map_Data.vRotation.x);
        protoElem->QueryFloatAttribute("y", &Map_Data.vRotation.y);
        protoElem->QueryFloatAttribute("z", &Map_Data.vRotation.z);

        m_MapDatas.push_back(Map_Data);
    }

    return S_OK;
}

HRESULT CData_Manager::Save_File(const _char* pFilePath)
{
    list<CGameObject*>* pObjectList =  m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_MapObject"));

    list<CMapObject*> pMapObjectList = {};

    for (auto pObjcet : *pObjectList)
    {
        pMapObjectList.push_back(dynamic_cast<CMapObject*>(pObjcet));
    }


    tinyxml2::XMLDocument doc;

    doc.InsertFirstChild(doc.NewDeclaration());

    // <Land name="BG_KEMAN">
    tinyxml2::XMLElement* land = doc.NewElement("Land");
    doc.InsertEndChild(land);


    for (auto pObject : pMapObjectList)
    {
        // <Model>
        tinyxml2::XMLElement* model = doc.NewElement("Model");
        land->InsertEndChild(model);

        // <PrototypeTag>텍스트</PrototypeTag>
        tinyxml2::XMLElement* proto = doc.NewElement("PrototypeTag");
        proto->SetText("Prototype_GameObject_Test");
        model->InsertEndChild(proto);

        // <Position x="0.0" y="0.0" z="0.0"/>
        tinyxml2::XMLElement* pos = doc.NewElement("Position");
        pos->SetAttribute("x", 0.1f);
        pos->SetAttribute("y", 0.1f);
        pos->SetAttribute("z", 0.1f);
        model->InsertEndChild(pos);
    }
    

    

    // 저장
    if (doc.SaveFile(pFilePath) != tinyxml2::XML_SUCCESS) {
      
        return E_FAIL;
    }



    return S_OK;
}

CData_Manager* CData_Manager::Creat()
{
    return new CData_Manager();
}

void CData_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
