#include "pch.h"
#include "Data_Manager.h"

#include "GameInstance.h"
#include "MapObject.h"

CData_Manager::CData_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    m_MapDatas.reserve(100);
    m_MapPreviewFileNames.reserve(50);
}

HRESULT CData_Manager::Load_MapData(const _char* pFilePath)
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

HRESULT CData_Manager::Save_MapData(const _char* pFileName)
{
    list<CGameObject*>* pObjectList =  m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"));

    if (nullptr == pObjectList)
        return E_FAIL;
  

    tinyxml2::XMLDocument doc;

    doc.InsertFirstChild(doc.NewDeclaration());

    // <Land name="BG_KEMAN">
    tinyxml2::XMLElement* land = doc.NewElement("Land");
    doc.InsertEndChild(land);

    for (auto pObjcet : *pObjectList)
    {
        CMapObject* pMapObject = dynamic_cast<CMapObject*>(pObjcet);

        if (nullptr == pMapObject)
            continue;

        // <Model>
        tinyxml2::XMLElement* model = doc.NewElement("Model");
        land->InsertEndChild(model);

        // <PrototypeTag>텍스트</PrototypeTag>
        tinyxml2::XMLElement* proto = doc.NewElement("PrototypeTag");
        proto->SetText(m_pGameInstance->WstringToUtf8(pMapObject->Get_PrototypeTag()).c_str());
        model->InsertEndChild(proto);

        // <Position x="0.0" y="0.0" z="0.0"/>
        tinyxml2::XMLElement* PositionElem = doc.NewElement("Position");

        _float3* pPosition = pMapObject->Get_Positon();
        PositionElem->SetAttribute("x", pPosition->x);
        PositionElem->SetAttribute("y", pPosition->y);
        PositionElem->SetAttribute("z", pPosition->z);
        model->InsertEndChild(PositionElem);

        // <Scale x="0.0" y="0.0" z="0.0"/>
        tinyxml2::XMLElement* ScaleElem = doc.NewElement("Scale");

        _float3* pScale = pMapObject->Get_Scale();
        ScaleElem->SetAttribute("x", pScale->x);
        ScaleElem->SetAttribute("y", pScale->y);
        ScaleElem->SetAttribute("z", pScale->z);
        model->InsertEndChild(ScaleElem);


        tinyxml2::XMLElement* RotationElem = doc.NewElement("Rotation");

        _float3* pRotation = pMapObject->Get_Rotation();
        RotationElem->SetAttribute("x", pRotation->x);
        RotationElem->SetAttribute("y", pRotation->y);
        RotationElem->SetAttribute("z", pRotation->z);
        model->InsertEndChild(RotationElem);

    }
    string strPath = "../Bin/Resources/Data/" + string(pFileName) + ".xml";

    // 저장
    if (doc.SaveFile(strPath.c_str()) != tinyxml2::XML_SUCCESS) {
      
        return E_FAIL;
    }



    return S_OK;
}

HRESULT CData_Manager::Load_PreviewTextures(const _char* pFilePath)
{

    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("MapObjectPreview_FileName");

    if (nullptr == root)
    {
        MSG_BOX("Failed to Find root");
        return E_FAIL;
    }


    for (auto* png = root->FirstChildElement("Name"); png; png = png->NextSiblingElement("Name"))
    {
        const _char* txt = png->GetText();

        if (nullptr != txt)
        {
            m_MapPreviewFileNames.push_back(CGameInstance::GetInstance()->Utf8ToWstring(txt));
        }
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
