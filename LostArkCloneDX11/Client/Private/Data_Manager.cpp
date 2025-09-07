#include "pch.h"
#include "Data_Manager.h"

#include "GameInstance.h"
#include "MapObject.h"
#include "Terrain.h"

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

    /* Terrain */
    TERRAIN_DATA Terrain_Data = {};

    for (auto* land = root->FirstChildElement("Terrain"); land; land = land->NextSiblingElement("Terrain"))
    {
        tinyxml2::XMLElement* protoElem = land->FirstChildElement("PrototypeTag");

        const _char* pPrototypeTag = protoElem->GetText();

        if (nullptr != pPrototypeTag)
        {
            Terrain_Data.strPrototypeTag = CGameInstance::GetInstance()->Utf8ToWstring(pPrototypeTag);
        }
        else
        {
            Terrain_Data.strPrototypeTag = L"";
        }

        protoElem = land->FirstChildElement("Position");
        if (nullptr == protoElem)
            return E_FAIL;
        protoElem->QueryFloatAttribute("x", &Terrain_Data.vPosition.x);
        protoElem->QueryFloatAttribute("y", &Terrain_Data.vPosition.y);
        protoElem->QueryFloatAttribute("z", &Terrain_Data.vPosition.z);

        protoElem = land->FirstChildElement("Rotation");
        if (nullptr == protoElem)
            return E_FAIL;
        protoElem->QueryFloatAttribute("x", &Terrain_Data.vRotation.x);
        protoElem->QueryFloatAttribute("y", &Terrain_Data.vRotation.y);
        protoElem->QueryFloatAttribute("z", &Terrain_Data.vRotation.z);

        m_TerrainDatas.push_back(Terrain_Data);
    }

    /* Map */
    MAP_DATA Map_Data = {};

    for (auto* land = root->FirstChildElement("Model"); land; land = land->NextSiblingElement("Model"))
    {
        tinyxml2::XMLElement* protoElem = land->FirstChildElement("PrototypeTag");

        const _char* pPrototypeTag = protoElem->GetText();

        if (nullptr != pPrototypeTag)
        {
            Map_Data.strPrototypeTag = CGameInstance::GetInstance()->Utf8ToWstring(pPrototypeTag);
        }
        else
        {
            Map_Data.strPrototypeTag = L"";
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
    list<CGameObject*>* pTerrainList =  m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Terrain"));
    list<CGameObject*>* pObjectList =  m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Layer_Background"));

    if (nullptr == pObjectList)
        return E_FAIL;

    if (nullptr == pTerrainList)
        return E_FAIL;
  

    tinyxml2::XMLDocument doc;

    doc.InsertFirstChild(doc.NewDeclaration());


    tinyxml2::XMLElement* land = doc.NewElement("Land");
    doc.InsertEndChild(land);

    for (auto pObjcet : *pTerrainList)
    {
        CTerrain* pTerrain= dynamic_cast<CTerrain*>(pObjcet);

        if (nullptr == pTerrain)
            continue;

        // <Model>
        tinyxml2::XMLElement* Terrain = doc.NewElement("Terrain");
        land->InsertEndChild(Terrain);

        tinyxml2::XMLElement* PrototypeElem = doc.NewElement("PrototypeTag");


        PrototypeElem->SetText(m_pGameInstance->WstringToUtf8(pTerrain->Get_PrototypeTag()).c_str());
        Terrain->InsertEndChild(PrototypeElem);

        // <Position x="0.0" y="0.0" z="0.0"/>
        tinyxml2::XMLElement* PositionElem = doc.NewElement("Position");

        _float4* pPosition = pTerrain->Get_Positon();
        PositionElem->SetAttribute("x", pPosition->x);
        PositionElem->SetAttribute("y", pPosition->y);
        PositionElem->SetAttribute("z", pPosition->z);
        Terrain->InsertEndChild(PositionElem);


        tinyxml2::XMLElement* RotationElem = doc.NewElement("Rotation");

        _float3* pRotation = pTerrain->Get_Rotation();
        RotationElem->SetAttribute("x", pRotation->x);
        RotationElem->SetAttribute("y", pRotation->y);
        RotationElem->SetAttribute("z", pRotation->z);
        Terrain->InsertEndChild(RotationElem);

    }

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

HRESULT CData_Manager::Load_SkillData(const _char* pFilePath)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Data");

    if (nullptr == root)
    {
        MSG_BOX("Failed to Find root");
        return E_FAIL;
    }

    for (auto* skill = root->FirstChildElement("Skill"); skill; skill = skill->NextSiblingElement("Skill"))
    {
        /*Skill 기본 정보 */
        SKILL_INFO Skill_Info = {};
        skill->QueryUnsignedAttribute("id", &Skill_Info.iSkillID);

        const char* pName;
        skill->QueryStringAttribute("name", &pName);
        Skill_Info.strSkillName = string(pName);

        skill->QueryStringAttribute("name_KR", &pName);
        Skill_Info.strSkillNameKR = CGameInstance::GetInstance()->Utf8ToWstring(pName);

        _uint iStance = {};
        skill->QueryUnsignedAttribute("stance", &iStance);
        Skill_Info.eStance = static_cast<STANCE>(iStance);

        /*Skill Stats */
        tinyxml2::XMLElement* Stats = skill->FirstChildElement("Stats");

        Stats->QueryFloatAttribute("coolTime", &Skill_Info.fCoolTime);

        Stats->QueryUnsignedAttribute("needMp", &Skill_Info.iNeedMp);

        Stats->QueryUnsignedAttribute("numAttack", &Skill_Info.iNumAttack);

        Stats->QueryFloatAttribute("idenGauge", &Skill_Info.fIdenGauge);

        /*Skill Flags */
        tinyxml2::XMLElement* Flags = skill->FirstChildElement("Flags");

        Flags->QueryBoolAttribute("countAttack", &Skill_Info.bCountAttack);

        Flags->QueryBoolAttribute("invincible", &Skill_Info.bInvincible);

        /*Skill Meta */
        tinyxml2::XMLElement* Meta = skill->FirstChildElement("Meta");

        const char* pSkillType;
        Meta->QueryStringAttribute("skillType", &pSkillType);

        if (!strcmp(pSkillType, "normal"))
            Skill_Info.eSkilltype = SKILL_TYPE::SKILL_NORAML;
        else if (!strcmp(pSkillType, "combo"))
            Skill_Info.eSkilltype = SKILL_TYPE::SKILL_COMBO;
        else if (!strcmp(pSkillType, "point"))
            Skill_Info.eSkilltype = SKILL_TYPE::SKILL_POINT;
        else if (!strcmp(pSkillType, "charge"))
            Skill_Info.eSkilltype = SKILL_TYPE::SKILL_CHARGE;
        else
            return E_FAIL;


        _uint iPartbreaker;
        Meta->QueryUnsignedAttribute("partbreaker", &iPartbreaker);

        Skill_Info.ePartbreaker = static_cast<PART_BREAKER>(iPartbreaker);

        const char* pStagger;
        Meta->QueryStringAttribute("staggerLevel", &pStagger);
        
        if (!strcmp(pStagger, "none"))
            Skill_Info.eStaggerLevel = STAGGER::STAGGER_NONE;
        else if (!strcmp(pStagger, "low"))
            Skill_Info.eStaggerLevel = STAGGER::STAGGER_LOW;
        else if (!strcmp(pStagger, "middle"))
            Skill_Info.eStaggerLevel = STAGGER::STAGGER_MIDDLE;
        else if (!strcmp(pStagger, "high"))
            Skill_Info.eStaggerLevel = STAGGER::STAGGER_HIGH;
        else
            return E_FAIL;

        const char* pAttackType;
        Meta->QueryStringAttribute("attackType", &pAttackType);

        if (!strcmp(pAttackType, "none"))
            Skill_Info.eAttackType = ATTACK_TYPE::NORMAL;
        else if (!strcmp(pAttackType, "head"))
            Skill_Info.eAttackType = ATTACK_TYPE::HEAD;
        else if (!strcmp(pAttackType, "back"))
            Skill_Info.eAttackType = ATTACK_TYPE::BACK;
        else
            return E_FAIL;

        const char* pSuperArmour;
        Meta->QueryStringAttribute("superArmour", &pSuperArmour);

        if (!strcmp(pSuperArmour, "none"))
            Skill_Info.eSuperArmour = SUPRE_ARMOUR_NONE;
        else if (!strcmp(pSuperArmour, "paralysis"))
            Skill_Info.eSuperArmour = SUPRE_ARMOUR_PARALYSIS;
        else if (!strcmp(pSuperArmour, "push"))
            Skill_Info.eSuperArmour = SUPRE_ARMOUR_PUSH;
        else if (!strcmp(pSuperArmour, "all"))
            Skill_Info.eSuperArmour = SUPRE_ARMOUR_ALL;
        else
            return E_FAIL;

        /*Skill Damage */
        tinyxml2::XMLElement* Hits = skill->FirstChildElement("Hit");

        Skill_Info.Damages.reserve(Skill_Info.iNumAttack);

        for (auto* Hits = skill->FirstChildElement("Hit"); Hits; Hits = Hits->NextSiblingElement("Hit"))
        {
            _float fDamage = {};
            Hits->QueryFloatAttribute("damage", &fDamage);
            Skill_Info.Damages.push_back(fDamage);
        }

        m_SkillDatas.push_back(Skill_Info);
    }

    return S_OK;
}

SKILL_INFO* CData_Manager::Get_SkillInfo_Prt(_uint iSkillID)
{
    if(m_SkillDatas.size() <= iSkillID)
        return nullptr;

    return &m_SkillDatas[iSkillID];
}

HRESULT CData_Manager::Load_AnimationData(const _char* pFilePath)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Data");

    if (nullptr == root)
    {
        MSG_BOX("Failed to Find root");
        return E_FAIL;
    }

    for (auto* Monster = root->FirstChildElement("Monster"); Monster; Monster = Monster->NextSiblingElement("Monster"))
    {
        vector<ANIMATION_DESC> Animations;

        for (_uint i = 0; i < ENUM_TO_INT(ANIMATIONSLOT::END); ++i)
        {
            ANIMATION_DESC Desc = {};

            Animations.push_back(Desc);
        }

        _uint iID = {};
        Monster->QueryUnsignedAttribute("id", &iID);

        for (auto* Animation = Monster->FirstChildElement("Animation"); Animation; Animation = Animation->NextSiblingElement("Animation"))
        {
            ANIMATION_DESC Anim_Desc = {};

            const char* pType;
            Animation->QueryStringAttribute("type", &pType);

#pragma region ANIM_SLOT
            if (!strcmp(pType, "respawn"))
                Anim_Desc.eSlot = ANIMATIONSLOT::RESPAWN;
            else if (!strcmp(pType, "idle"))
                Anim_Desc.eSlot = ANIMATIONSLOT::IDLE;
            else if (!strcmp(pType, "idle_battle"))
                Anim_Desc.eSlot = ANIMATIONSLOT::IDLE_BATTLE;
            else if (!strcmp(pType, "walk"))
                Anim_Desc.eSlot = ANIMATIONSLOT::WALK;
            else if (!strcmp(pType, "run"))
                Anim_Desc.eSlot = ANIMATIONSLOT::RUN;
            else if (!strcmp(pType, "turn_r"))
                Anim_Desc.eSlot = ANIMATIONSLOT::TURN_R;
            else if (!strcmp(pType, "turn_l"))
                Anim_Desc.eSlot = ANIMATIONSLOT::TURN_L;
            else if (!strcmp(pType, "dead"))
                Anim_Desc.eSlot = ANIMATIONSLOT::DEAD;
            else if (!strcmp(pType, "dead_loop"))
                Anim_Desc.eSlot = ANIMATIONSLOT::DEAD_LOOP;
            else if (!strcmp(pType, "attack_1"))
                Anim_Desc.eSlot = ANIMATIONSLOT::ATTACK_1;
            else if (!strcmp(pType, "attack_2"))
                Anim_Desc.eSlot = ANIMATIONSLOT::ATTACK_2;
            else if (!strcmp(pType, "attack_3"))
                Anim_Desc.eSlot = ANIMATIONSLOT::ATTACK_3;
            else if (!strcmp(pType, "attack_4"))
                Anim_Desc.eSlot = ANIMATIONSLOT::ATTACK_4;
            else if (!strcmp(pType, "attack_5"))
                Anim_Desc.eSlot = ANIMATIONSLOT::ATTACK_5;
            else if (!strcmp(pType, "critical_start"))
                Anim_Desc.eSlot = ANIMATIONSLOT::CRITICAL_START;
            else if (!strcmp(pType, "critical_loop"))
                Anim_Desc.eSlot = ANIMATIONSLOT::CRITICAL_LOOP;
            else if (!strcmp(pType, "critical_end"))
                Anim_Desc.eSlot = ANIMATIONSLOT::CRITICAL_END;
            else if (!strcmp(pType, "bound"))
                Anim_Desc.eSlot = ANIMATIONSLOT::BOUND;
            else if (!strcmp(pType, "bound_hit"))
                Anim_Desc.eSlot = ANIMATIONSLOT::BOUND_HIT;
            else if (!strcmp(pType, "bound_land"))
                Anim_Desc.eSlot = ANIMATIONSLOT::BOUND_LAND;
            else if (!strcmp(pType, "ground"))
                Anim_Desc.eSlot = ANIMATIONSLOT::GROUND;
            else if (!strcmp(pType, "standup"))
                Anim_Desc.eSlot = ANIMATIONSLOT::STANDUP;
            else if (!strcmp(pType, "knokdown"))              // enum 철자 그대로 매칭
                Anim_Desc.eSlot = ANIMATIONSLOT::KNOKDOWN;
            else if (!strcmp(pType, "knokdown_land"))
                Anim_Desc.eSlot = ANIMATIONSLOT::KNOKDOWN_LAND;
            else if (!strcmp(pType, "twist_knockdown"))       // enum: TWIST_KNOCKDOWN
                Anim_Desc.eSlot = ANIMATIONSLOT::TWIST_KNOCKDOWN;
            else if (!strcmp(pType, "twist_knockdown_land"))
                Anim_Desc.eSlot = ANIMATIONSLOT::TWIST_KNOCKDOWN_LAND;
            else
                return E_FAIL;
#pragma endregion

            /* 애니메이션 인덱스 */
            Animation->QueryUnsignedAttribute("index", &Anim_Desc.iIndex);

            /* Loop */
            Animation->QueryBoolAttribute("loop", &Anim_Desc.bLoop);

            /* 선형보간할 시간 */
            Animation->QueryFloatAttribute("lerp", &Anim_Desc.fLerpTime);

            Animations[ENUM_TO_INT(Anim_Desc.eSlot)] = Anim_Desc;
        }

        m_AnimationData.push_back(Animations);
    }


    return S_OK;
}

ANIMATION_DESC& CData_Manager::Get_AnimationIndex(_uint iMonsterID, ANIMATIONSLOT eSlot)
{
    if (m_AnimationData.size() <= iMonsterID)
        return m_AnimationData[0][0];

    return m_AnimationData[iMonsterID][ENUM_TO_INT(eSlot)];
}

CData_Manager* CData_Manager::Create()
{
    return new CData_Manager();
}

void CData_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
