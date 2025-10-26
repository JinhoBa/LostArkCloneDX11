#include "pch.h"
#include "Effect_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"
#include "Monster.h"
#include "Kamen.h"
#include "Player.h"
#include "Effect_Ground.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"

CEffect_Manager::CEffect_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{pDevice, pContext}
{
}

CEffect_Manager::CEffect_Manager(const CEffect_Manager& Prototype)
    :CGameObject{Prototype}
{
}

HRESULT CEffect_Manager::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Manager::Initialize(void* pArg)
{
    m_isScreenBlur = false;
    m_vScreenBlurTime = _float2(0.f, 0.5f);

    if (FAILED(Add_Components()))
        return E_FAIL;

    for (_uint i = 0; i < 10; i++)
    {
        CEffect_Ground* pEffect = dynamic_cast<CEffect_Ground*>(m_pGameInstance->Clone_Prototype(
            PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Ground")));


        m_GroundEffects.push_back(
            pEffect
        );
    }

    for (_uint i = 0; i < 10; i++)
    {
        m_MeshEffects.push_back(
            dynamic_cast<CEffect_Mesh*>(m_pGameInstance->Clone_Prototype(
                PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Mesh")))
        );
    }

    for (_uint i = 0; i < 10; i++)
    {
        m_ParticleEffects.push_back(
            dynamic_cast<CEffect_Particle*>(m_pGameInstance->Clone_Prototype(
                PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Particle")))
        );
    }

    if (FAILED(Load_Mesh_Data("../Bin/Resources/Data/Effect/Player_Effects.xml", CHARACTER::PLAYER)))
        return E_FAIL;

    if (FAILED(Load_Ground_Data("../Bin/Resources/Data/Effect/Player_GroundEffects.xml", CHARACTER::PLAYER)))
        return E_FAIL;

    if (FAILED(Load_Particle_Data("../Bin/Resources/Data/Effect/Player_ParticleEffects.xml", CHARACTER::PLAYER)))
        return E_FAIL;

    if (FAILED(Load_EffectTrack(CHARACTER::PLAYER, "../Bin/Resources/Data/Effect/Player_Effects_Track.xml")))
        return E_FAIL;

    if (FAILED(Load_Mesh_Data("../Bin/Resources/Data/Effect/Kamen_Effects.xml", CHARACTER::BOSS)))
        return E_FAIL;

    if (FAILED(Load_EffectTrack(CHARACTER::BOSS, "../Bin/Resources/Data/Effect/Kamen_Effects_Track.xml")))
        return E_FAIL;

    return S_OK;
}

void CEffect_Manager::Priority_Update(_float fTimeDelta)
{
    for (auto& pEffect : m_pActiveMeshEffects)
    {
        pEffect->Priority_Update(fTimeDelta);
    }

    for (auto& pEffect : m_pActiveGroundEffects)
    {
        pEffect->Priority_Update(fTimeDelta);
    }

    for (auto& pEffect : m_pActiveParticleEffects)
    {
        pEffect->Priority_Update(fTimeDelta);
    }
}

void CEffect_Manager::Update(_float fTimeDelta)
{
    for (auto& pEffect : m_pActiveMeshEffects)
    {
        pEffect->Update(fTimeDelta);
    }

    for (auto& pEffect : m_pActiveGroundEffects)
    {
        pEffect->Update(fTimeDelta);
    }

    for (auto& pEffect : m_pActiveParticleEffects)
    {
        pEffect->Update(fTimeDelta);
    }

    if (m_isScreenBlur)
    {
        m_vScreenBlurTime.x += fTimeDelta;

        m_pGameInstance->BlurBackBuffer(1.f - abs(m_vScreenBlurTime.x / m_vScreenBlurTime.y * 2.f - 1.f));

        if (m_vScreenBlurTime.x >= m_vScreenBlurTime.y)
        {
            m_vScreenBlurTime.x = 0.f;
            m_isScreenBlur = false;
        }
    }
}

void CEffect_Manager::Late_Update(_float fTimeDelta)
{
    auto iter = m_pActiveMeshEffects.begin();
    for (;iter != m_pActiveMeshEffects.end(); )
    {
        if ((*iter)->isDead())
        {
            (*iter)->Reset();
            m_MeshEffects.push_back(*iter);
            iter = m_pActiveMeshEffects.erase(iter);
        }
        else
        {
            (*iter)->Late_Update(fTimeDelta);
            ++iter;
        }
    }

    iter = m_pActiveGroundEffects.begin();
    for (;iter != m_pActiveGroundEffects.end(); )
    {
        if ((*iter)->isDead())
        {
            (*iter)->Reset();
            m_GroundEffects.push_back(*iter);
            iter = m_pActiveGroundEffects.erase(iter);
        }
        else
        {
            (*iter)->Late_Update(fTimeDelta);
            ++iter;
        }
    }

    iter = m_pActiveParticleEffects.begin();
    for (;iter != m_pActiveParticleEffects.end(); )
    {
        if ((*iter)->isDead())
        {
            (*iter)->Reset();
            m_ParticleEffects.push_back(*iter);
            iter = m_pActiveParticleEffects.erase(iter);
        }
        else
        {
            (*iter)->Late_Update(fTimeDelta);
            ++iter;
        }
    }
}

HRESULT CEffect_Manager::Render()
{
    return S_OK;
}

void CEffect_Manager::Add_Effects(EFFECT eType, _uint iEffectID, const _float4x4* pWorldMatirx, CHARACTER eCharacter)
{
    switch (eType)
    {
    case Client::EFFECT::BILLBORD:
        break;

    case Client::EFFECT::TRAIL:
        break;

    case Client::EFFECT::GROUND:
        m_GroundEffects.front()->Start(pWorldMatirx, &(m_GroundEffectData[ENUM_TO_INT(eCharacter)][iEffectID]));

        m_pActiveGroundEffects.push_back(m_GroundEffects.front());

        m_GroundEffects.pop_front();
        break;

    case Client::EFFECT::PARTICLE:
        m_ParticleEffects.front()->Start(pWorldMatirx, &(m_ParticleEffect_Datas[ENUM_TO_INT(eCharacter)][iEffectID]));

        m_pActiveParticleEffects.push_back(m_ParticleEffects.front());

        m_ParticleEffects.pop_front();
        break;

    case Client::EFFECT::MESH:
        m_MeshEffects.front()->Start(pWorldMatirx, &(m_MeshEffect_Datas[ENUM_TO_INT(eCharacter)][iEffectID]));

        m_pActiveMeshEffects.push_back(m_MeshEffects.front());

        m_MeshEffects.pop_front();
        break;
    }
        
}

void CEffect_Manager::Start_ScreenBlur(_float fTime)
{
    m_isScreenBlur = true;

    m_vScreenBlurTime.x = 0.f;
    m_vScreenBlurTime.y = fTime;
}

const vector<EFFECT_EVENT_DESC>& CEffect_Manager::Get_EffectTrack(CHARACTER eType, _uint iTrackIndex)
{
    return m_EffectEvents[ENUM_TO_INT(eType)][iTrackIndex];
}

const vector<CAMERA_SHAKE_EVENT_DESC>& CEffect_Manager::Get_Camera_Track(_uint iTrackIndex)
{
    return m_CameraShakeEvents[iTrackIndex];
}

const vector<BLUR_EVENT_DESC>& CEffect_Manager::Get_BlurTrack(_uint iTrackIndex)
{
    return m_BlurEvents[iTrackIndex];
}

HRESULT CEffect_Manager::Load_Mesh_Data(const _char* pFilePath, CHARACTER eType)
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

    for (auto* Effect = root->FirstChildElement("Effect"); Effect; Effect = Effect->NextSiblingElement("Effect"))
    {
        /*Effect Data*/
        EFFECT_MESH_DESC Desc = {};

#pragma region MESH
        /*Effect Texture */
        tinyxml2::XMLElement* Mesh = Effect->FirstChildElement("Mesh");

        Mesh->QueryUnsignedAttribute("Index", &Desc.Mesh_Data.iMeshIndex);
#pragma endregion

#pragma region TEXTURE
        tinyxml2::XMLElement* Texture = Effect->FirstChildElement("Texture");

        const char* pName;
        Texture->QueryStringAttribute("DiffuseTexture", &pName);
        Desc.strDiffuseTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("MaskTexture", &pName);
        Desc.strMaskTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("NoiseTexture", &pName);
        Desc.strNoiseTexture = m_pGameInstance->Utf8ToWstring(pName);
#pragma endregion

#pragma region SCALE
        tinyxml2::XMLElement* Scale = Effect->FirstChildElement("Scale");

        Scale->QueryBoolAttribute("Use", &Desc.Mesh_Data.isUseScale);

        Scale->QueryFloatAttribute("StartX", &Desc.Mesh_Data.vStartScale.x);
        Scale->QueryFloatAttribute("StartY", &Desc.Mesh_Data.vStartScale.y);
        Scale->QueryFloatAttribute("StartZ", &Desc.Mesh_Data.vStartScale.z);

        Scale->QueryFloatAttribute("EndX", &Desc.Mesh_Data.vEndScale.x);
        Scale->QueryFloatAttribute("EndY", &Desc.Mesh_Data.vEndScale.y);
        Scale->QueryFloatAttribute("EndZ", &Desc.Mesh_Data.vEndScale.z);
#pragma endregion

#pragma region ROTATION
        tinyxml2::XMLElement* Rotation = Effect->FirstChildElement("Rotation");

        Rotation->QueryFloatAttribute("StartX", &Desc.Mesh_Data.vStartRotation.x);
        Rotation->QueryFloatAttribute("StartY", &Desc.Mesh_Data.vStartRotation.y);
        Rotation->QueryFloatAttribute("StartZ", &Desc.Mesh_Data.vStartRotation.z);

        Rotation->QueryFloatAttribute("EndX", &Desc.Mesh_Data.vEndRotation.x);
        Rotation->QueryFloatAttribute("EndY", &Desc.Mesh_Data.vEndRotation.y);
        Rotation->QueryFloatAttribute("EndZ", &Desc.Mesh_Data.vEndRotation.z);
#pragma endregion

#pragma region POSITION
        tinyxml2::XMLElement* Position = Effect->FirstChildElement("Position");

        Position->QueryFloatAttribute("X", &Desc.Mesh_Data.vPosition.x);
        Position->QueryFloatAttribute("Y", &Desc.Mesh_Data.vPosition.y);
        Position->QueryFloatAttribute("Z", &Desc.Mesh_Data.vPosition.z);
#pragma endregion

#pragma region PIVOT
        tinyxml2::XMLElement* Pivot = Effect->FirstChildElement("Pivot");

        Pivot->QueryFloatAttribute("Speed", &Desc.Mesh_Data.fSpeed);

        Pivot->QueryFloatAttribute("X", &Desc.Mesh_Data.vPivot.x);
        Pivot->QueryFloatAttribute("Y", &Desc.Mesh_Data.vPivot.y);
        Pivot->QueryFloatAttribute("Z", &Desc.Mesh_Data.vPivot.z);
#pragma endregion

#pragma region LIFETIME
        tinyxml2::XMLElement* LifeTime = Effect->FirstChildElement("LifeTime");

        LifeTime->QueryBoolAttribute("Loop", &Desc.Mesh_Data.isLoop);
        LifeTime->QueryFloatAttribute("X", &Desc.Mesh_Data.vLifeTime.x);
        LifeTime->QueryFloatAttribute("Y", &Desc.Mesh_Data.vLifeTime.y);
#pragma endregion

#pragma region SHADER
        tinyxml2::XMLElement* PassIndex = Effect->FirstChildElement("PassIndex");

        PassIndex->QueryUnsignedAttribute("Index", &Desc.Mesh_Data.iPassIndex);


        tinyxml2::XMLElement* Diffuse = Effect->FirstChildElement("Diffuse");

        Diffuse->QueryFloatAttribute("ScrollSpeedU", &Desc.Mesh_Data.fDiffuseScrollSpeedU);
        Diffuse->QueryFloatAttribute("ScrollSpeedV", &Desc.Mesh_Data.fDiffuseScrollSpeedV);


        tinyxml2::XMLElement* Mask = Effect->FirstChildElement("Mask");

        Mask->QueryFloatAttribute("ScrollSpeedU", &Desc.Mesh_Data.fMaskScrollSpeedU);
        Mask->QueryFloatAttribute("ScrollSpeedV", &Desc.Mesh_Data.fMaskScrollSpeedV);

#pragma endregion

        m_MeshEffect_Datas[ENUM_TO_INT(eType)].push_back(Desc);
    }


    return S_OK;
}

HRESULT CEffect_Manager::Load_Ground_Data(const _char* pFilePath, CHARACTER eType)
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

    for (auto* Effect = root->FirstChildElement("Effect"); Effect; Effect = Effect->NextSiblingElement("Effect"))
    {
        /*Effect Data*/
        EFFECT_GROUND_DESC Desc = {};

#pragma region TEXTURE
        tinyxml2::XMLElement* Texture = Effect->FirstChildElement("Texture");

        const char* pName;
        Texture->QueryStringAttribute("DiffuseTexture", &pName);
        Desc.strDiffuseTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("MaskTexture", &pName);
        Desc.strMaskTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("NoiseTexture", &pName);
        Desc.strNoiseTexture = m_pGameInstance->Utf8ToWstring(pName);
#pragma endregion

#pragma region SCALE
        tinyxml2::XMLElement* Scale = Effect->FirstChildElement("Scale");
        _uint iLerp = {};
        Scale->QueryUnsignedAttribute("Lerp", &iLerp);
        Desc.Ground_Data.eLerpType = static_cast<LERP>(iLerp);
        Scale->QueryFloatAttribute("x", &Desc.Ground_Data.vSize.x);
        Scale->QueryFloatAttribute("y", &Desc.Ground_Data.vSize.y);
#pragma endregion

#pragma region ROTATION
        tinyxml2::XMLElement* Rotation = Effect->FirstChildElement("Rotation");

        Rotation->QueryBoolAttribute("Use", &Desc.Ground_Data.isRotation);

        Rotation->QueryFloatAttribute("StartX", &Desc.Ground_Data.vStartRotation.x);
        Rotation->QueryFloatAttribute("StartY", &Desc.Ground_Data.vStartRotation.y);
        Rotation->QueryFloatAttribute("StartZ", &Desc.Ground_Data.vStartRotation.z);

        Rotation->QueryFloatAttribute("EndX", &Desc.Ground_Data.vEndRotation.x);
        Rotation->QueryFloatAttribute("EndY", &Desc.Ground_Data.vEndRotation.y);
        Rotation->QueryFloatAttribute("EndZ", &Desc.Ground_Data.vEndRotation.z);
#pragma endregion

#pragma region POSITION
        tinyxml2::XMLElement* Position = Effect->FirstChildElement("Position");

        Position->QueryFloatAttribute("X", &Desc.Ground_Data.vPosition.x);
        Position->QueryFloatAttribute("Y", &Desc.Ground_Data.vPosition.y);
        Position->QueryFloatAttribute("Z", &Desc.Ground_Data.vPosition.z);
#pragma endregion

#pragma region PIVOT
        tinyxml2::XMLElement* Pivot = Effect->FirstChildElement("Pivot");

        Pivot->QueryFloatAttribute("Speed", &Desc.Ground_Data.fSpeed);

        Pivot->QueryFloatAttribute("X", &Desc.Ground_Data.vPivot.x);
        Pivot->QueryFloatAttribute("Y", &Desc.Ground_Data.vPivot.y);
        Pivot->QueryFloatAttribute("Z", &Desc.Ground_Data.vPivot.z);
#pragma endregion

#pragma region LIFETIME
        tinyxml2::XMLElement* LifeTime = Effect->FirstChildElement("LifeTime");

        LifeTime->QueryFloatAttribute("X", &Desc.Ground_Data.vLifeTime.x);
        LifeTime->QueryFloatAttribute("Y", &Desc.Ground_Data.vLifeTime.y);
#pragma endregion

#pragma region SHADER
        tinyxml2::XMLElement* PassIndex = Effect->FirstChildElement("PassIndex");

        PassIndex->QueryUnsignedAttribute("Index", &Desc.Ground_Data.iPassIndex);


#pragma endregion

        m_GroundEffectData[ENUM_TO_INT(eType)].push_back(Desc);
    }


    return S_OK;
}

HRESULT CEffect_Manager::Load_Particle_Data(const _char* pFilePath, CHARACTER eType)
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

    for (auto* Effect = root->FirstChildElement("Effect"); Effect; Effect = Effect->NextSiblingElement("Effect"))
    {
        /*Effect Data*/
        EFFECT_PARTICLE_DESC Desc = {};

#pragma region TEXTURE
        tinyxml2::XMLElement* Texture = Effect->FirstChildElement("Texture");

        const char* pName;
        Texture->QueryStringAttribute("DiffuseTexture", &pName);
        Desc.strDiffuseTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("MaskTexture", &pName);
        Desc.strMaskTexture = m_pGameInstance->Utf8ToWstring(pName);

        Texture->QueryStringAttribute("NoiseTexture", &pName);
        Desc.strNoiseTexture = m_pGameInstance->Utf8ToWstring(pName);
#pragma endregion

#pragma region SCALE
        tinyxml2::XMLElement* Scale = Effect->FirstChildElement("Scale");
        _uint iType = {};
        Scale->QueryUnsignedAttribute("Type", &iType);
        Desc.Data.eType = static_cast<PARTICLE>(iType);
        
        Scale->QueryFloatAttribute("x", &Desc.Data.vSize.x);
        Scale->QueryFloatAttribute("y", &Desc.Data.vSize.y);
#pragma endregion

#pragma region SLICE
        tinyxml2::XMLElement* Tile = Effect->FirstChildElement("Tile");
        Tile->QueryFloatAttribute("x", &Desc.Data.vTileSize.x);
        Tile->QueryFloatAttribute("y", &Desc.Data.vTileSize.y);

        Effect->InsertEndChild(Tile);
#pragma endregion

#pragma region INFO
        tinyxml2::XMLElement* Info = Effect->FirstChildElement("Info");

        Info->QueryBoolAttribute("Loop", &Desc.Data.isLoop);
        Info->QueryUnsignedAttribute("Pass", &Desc.Data.iPassIndex);
        Info->QueryUnsignedAttribute("NumInstance", &Desc.Data.iNumInstance);
        Info->QueryFloatAttribute("ActiveTime", &Desc.Data.fActiveTime);

#pragma endregion

#pragma region CENTER
        tinyxml2::XMLElement* Position = Effect->FirstChildElement("Center");

        Position->QueryFloatAttribute("X", &Desc.Data.vCenter.x);
        Position->QueryFloatAttribute("Y", &Desc.Data.vCenter.y);
        Position->QueryFloatAttribute("Z", &Desc.Data.vCenter.z);
#pragma endregion

#pragma region PIVOT
        tinyxml2::XMLElement* Pivot = Effect->FirstChildElement("Pivot");


        Pivot->QueryFloatAttribute("X", &Desc.Data.vPivot.x);
        Pivot->QueryFloatAttribute("Y", &Desc.Data.vPivot.y);
        Pivot->QueryFloatAttribute("Z", &Desc.Data.vPivot.z);
#pragma endregion

#pragma region RANGE
        tinyxml2::XMLElement* Range = Effect->FirstChildElement("Range");

        Range->QueryFloatAttribute("X", &Desc.Data.vRange.x);
        Range->QueryFloatAttribute("Y", &Desc.Data.vRange.y);
        Range->QueryFloatAttribute("Z", &Desc.Data.vRange.z);
#pragma endregion

#pragma region LIFETIME
        tinyxml2::XMLElement* LifeTime = Effect->FirstChildElement("LifeTime");

        LifeTime->QueryFloatAttribute("X", &Desc.Data.vLifeTime.x);
        LifeTime->QueryFloatAttribute("Y", &Desc.Data.vLifeTime.y);
#pragma endregion

#pragma region SPEED
        tinyxml2::XMLElement* Speed = Effect->FirstChildElement("Speed");

        Speed->QueryFloatAttribute("X", &Desc.Data.vSpeed.x);
        Speed->QueryFloatAttribute("Y", &Desc.Data.vSpeed.y);
#pragma endregion

        m_ParticleEffect_Datas[ENUM_TO_INT(eType)].push_back(Desc);
    }


    return S_OK;
}

HRESULT CEffect_Manager::Load_EffectTrack(CHARACTER eType, const _char* pFilePath)
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

    for (auto* Skill = root->FirstChildElement("Skill"); Skill; Skill = Skill->NextSiblingElement("Skill"))
    {
        vector<EFFECT_EVENT_DESC> EffectDesces;
        vector<CAMERA_SHAKE_EVENT_DESC> CameraShakeDesces;
        vector<BLUR_EVENT_DESC> BlurDesces;

        for (auto* Effect = Skill->FirstChildElement("Effect"); Effect; Effect = Effect->NextSiblingElement("Effect"))
        {
            EFFECT_EVENT_DESC Effect_Desc = {};

            const char* pName;
            Effect->QueryStringAttribute("type", &pName);

            if (!strcmp(pName, "mesh"))
                Effect_Desc.eType = EFFECT::MESH;
            else if (!strcmp(pName, "ground"))
                Effect_Desc.eType = EFFECT::GROUND;
            else if (!strcmp(pName, "particle"))
                Effect_Desc.eType = EFFECT::PARTICLE;
            else if (!strcmp(pName, "none"))
                Effect_Desc.eType = EFFECT::MESH;
            
            Effect->QueryFloatAttribute("keyframe", &Effect_Desc.fKeyFrame);

            Effect->QueryUnsignedAttribute("effectID", &Effect_Desc.iID);

            EffectDesces.push_back(Effect_Desc);
        }
        
        m_EffectEvents[ENUM_TO_INT(eType)].push_back(EffectDesces);

        for (auto* Camera = Skill->FirstChildElement("CameraShake"); Camera; Camera = Camera->NextSiblingElement("CameraShake"))
        {
            CAMERA_SHAKE_EVENT_DESC CameraShake_Desc = {};

            Camera->QueryFloatAttribute("keyframe", &CameraShake_Desc.fKeyFrame);
            Camera->QueryFloatAttribute("time", &CameraShake_Desc.fTime);

            CameraShakeDesces.push_back(CameraShake_Desc);
        }
        m_CameraShakeEvents.push_back(CameraShakeDesces);

        for (auto* Blur = Skill->FirstChildElement("Blur"); Blur; Blur = Blur->NextSiblingElement("Blur"))
        {
            BLUR_EVENT_DESC Blur_Desc = {};

            Blur->QueryFloatAttribute("keyframe", &Blur_Desc.fKeyFrame);
            Blur->QueryFloatAttribute("time", &Blur_Desc.fTime);

            BlurDesces.push_back(Blur_Desc);
        }
        m_BlurEvents.push_back(BlurDesces);
    }

    return S_OK;
}

HRESULT CEffect_Manager::Save_Effect(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID)
{
    if (EFFECT::MESH == eType)
    {
        if(FAILED(Save_Mesh(eType, pArg, pFilePath, iID)))
            return E_FAIL;
    }
    else if (EFFECT::GROUND == eType)
    {
        if (FAILED(Save_Ground(eType, pArg, pFilePath, iID)))
            return E_FAIL;
    }
    else if (EFFECT::PARTICLE == eType)
    {
        if (FAILED(Save_Particle(eType, pArg, pFilePath, iID)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CEffect_Manager::Save_Particle(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Data");

    EFFECT_PARTICLE_DESC* pDesc = static_cast<EFFECT_PARTICLE_DESC*>(pArg);

    tinyxml2::XMLElement* Effect = xmlDoc.NewElement("Effect");

    Effect->SetAttribute("id", iID);

    _uint itype = ENUM_TO_INT(eType);
    Effect->SetAttribute("type", itype);

#pragma region TEXTURE
    tinyxml2::XMLElement* Texture = xmlDoc.NewElement("Texture");

    Texture->SetAttribute("DiffuseTexture", m_pGameInstance->WstringToUtf8(pDesc->strDiffuseTexture).c_str());
    Texture->SetAttribute("MaskTexture", m_pGameInstance->WstringToUtf8(pDesc->strMaskTexture).c_str());
    Texture->SetAttribute("NoiseTexture", m_pGameInstance->WstringToUtf8(pDesc->strNoiseTexture).c_str());

    Effect->InsertEndChild(Texture);
#pragma endregion

#pragma region SCALE
    tinyxml2::XMLElement* Scale = xmlDoc.NewElement("Scale");

    _uint iType = ENUM_TO_INT(pDesc->Data.eType);
    Scale->SetAttribute("Type", iType);

    Scale->SetAttribute("x", pDesc->Data.vSize.x);
    Scale->SetAttribute("y", pDesc->Data.vSize.y);

    Effect->InsertEndChild(Scale);
#pragma endregion

#pragma region SLICE
    tinyxml2::XMLElement* Tile = xmlDoc.NewElement("Tile");

    Tile->SetAttribute("x", pDesc->Data.vTileSize.x);
    Tile->SetAttribute("y", pDesc->Data.vTileSize.y);

    Effect->InsertEndChild(Tile);
#pragma endregion

#pragma region INFO
    tinyxml2::XMLElement* Info = xmlDoc.NewElement("Info");

    Info->SetAttribute("Loop", pDesc->Data.isLoop);
    Info->SetAttribute("Pass", pDesc->Data.iPassIndex);
    Info->SetAttribute("NumInstance", pDesc->Data.iNumInstance);
    Info->SetAttribute("ActiveTime", pDesc->Data.fActiveTime);

    Effect->InsertEndChild(Info);

#pragma endregion

#pragma region POSITION
    tinyxml2::XMLElement* Position = xmlDoc.NewElement("Center");

    Position->SetAttribute("X", pDesc->Data.vCenter.x);
    Position->SetAttribute("Y", pDesc->Data.vCenter.y);
    Position->SetAttribute("Z", pDesc->Data.vCenter.z);

    Effect->InsertEndChild(Position);
#pragma endregion

#pragma region PIVOT
    tinyxml2::XMLElement* Pivot = xmlDoc.NewElement("Pivot");

    Pivot->SetAttribute("X", pDesc->Data.vPivot.x);
    Pivot->SetAttribute("Y", pDesc->Data.vPivot.y);
    Pivot->SetAttribute("Z", pDesc->Data.vPivot.z);

    Effect->InsertEndChild(Pivot);
#pragma endregion

#pragma region RANGE
    tinyxml2::XMLElement* Range = xmlDoc.NewElement("Range");

    Range->SetAttribute("X", pDesc->Data.vRange.x);
    Range->SetAttribute("Y", pDesc->Data.vRange.y);
    Range->SetAttribute("Z", pDesc->Data.vRange.z);

    Effect->InsertEndChild(Range);
#pragma endregion

#pragma region LIFETIME
    tinyxml2::XMLElement* LifeTime = xmlDoc.NewElement("LifeTime");

    LifeTime->SetAttribute("X", pDesc->Data.vLifeTime.x);
    LifeTime->SetAttribute("Y", pDesc->Data.vLifeTime.y);

    Effect->InsertEndChild(LifeTime);
#pragma endregion

#pragma region SPEED
    tinyxml2::XMLElement* Speed = xmlDoc.NewElement("Speed");

    Speed->SetAttribute("X", pDesc->Data.vSpeed.x);
    Speed->SetAttribute("Y", pDesc->Data.vSpeed.y);

    Effect->InsertEndChild(Speed);
#pragma endregion



    root->InsertEndChild(Effect);

    if (tinyxml2::XML_SUCCESS != xmlDoc.SaveFile(pFilePath))
    {
        MSG_BOX("Failed to Save Effect data");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CEffect_Manager::Save_Mesh(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Data");

    EFFECT_MESH_DESC* pDesc = static_cast<EFFECT_MESH_DESC*>(pArg);

    tinyxml2::XMLElement* Effect = xmlDoc.NewElement("Effect");

    Effect->SetAttribute("id", iID);

    _uint itype = ENUM_TO_INT(eType);
    Effect->SetAttribute("type", itype);

#pragma region MESH
    tinyxml2::XMLElement* Mesh = xmlDoc.NewElement("Mesh");

    Mesh->SetAttribute("Index", pDesc->Mesh_Data.iMeshIndex);

    Effect->InsertEndChild(Mesh);
#pragma endregion

#pragma region TEXTURE
    tinyxml2::XMLElement* Texture = xmlDoc.NewElement("Texture");

    Texture->SetAttribute("DiffuseTexture", m_pGameInstance->WstringToUtf8(pDesc->strDiffuseTexture).c_str());
    Texture->SetAttribute("MaskTexture", m_pGameInstance->WstringToUtf8(pDesc->strMaskTexture).c_str());
    Texture->SetAttribute("NoiseTexture", m_pGameInstance->WstringToUtf8(pDesc->strNoiseTexture).c_str());

    Effect->InsertEndChild(Texture);
#pragma endregion

#pragma region SCALE
    tinyxml2::XMLElement* Scale = xmlDoc.NewElement("Scale");

    Scale->SetAttribute("Use", pDesc->Mesh_Data.isUseScale);

    Scale->SetAttribute("StartX", pDesc->Mesh_Data.vStartScale.x);
    Scale->SetAttribute("StartY", pDesc->Mesh_Data.vStartScale.y);
    Scale->SetAttribute("StartZ", pDesc->Mesh_Data.vStartScale.z);

    Scale->SetAttribute("EndX", pDesc->Mesh_Data.vEndScale.x);
    Scale->SetAttribute("EndY", pDesc->Mesh_Data.vEndScale.y);
    Scale->SetAttribute("EndZ", pDesc->Mesh_Data.vEndScale.z);

    Effect->InsertEndChild(Scale);
#pragma endregion

#pragma region ROTATION
    tinyxml2::XMLElement* Rotation = xmlDoc.NewElement("Rotation");

    Rotation->SetAttribute("Use", pDesc->Mesh_Data.isUseRotation);

    Rotation->SetAttribute("StartX", pDesc->Mesh_Data.vStartRotation.x);
    Rotation->SetAttribute("StartY", pDesc->Mesh_Data.vStartRotation.y);
    Rotation->SetAttribute("StartZ", pDesc->Mesh_Data.vStartRotation.z);

    Rotation->SetAttribute("EndX", pDesc->Mesh_Data.vEndRotation.x);
    Rotation->SetAttribute("EndY", pDesc->Mesh_Data.vEndRotation.y);
    Rotation->SetAttribute("EndZ", pDesc->Mesh_Data.vEndRotation.z);

    Effect->InsertEndChild(Rotation);
#pragma endregion

#pragma region POSITION
    tinyxml2::XMLElement* Position = xmlDoc.NewElement("Position");

    Position->SetAttribute("X", pDesc->Mesh_Data.vPosition.x);
    Position->SetAttribute("Y", pDesc->Mesh_Data.vPosition.y);
    Position->SetAttribute("Z", pDesc->Mesh_Data.vPosition.z);

    Effect->InsertEndChild(Position);
#pragma endregion

#pragma region PIVOT
    tinyxml2::XMLElement* Pivot = xmlDoc.NewElement("Pivot");

    Pivot->SetAttribute("Speed", pDesc->Mesh_Data.fSpeed);

    Pivot->SetAttribute("X", pDesc->Mesh_Data.vPivot.x);
    Pivot->SetAttribute("Y", pDesc->Mesh_Data.vPivot.y);
    Pivot->SetAttribute("Z", pDesc->Mesh_Data.vPivot.z);

    Effect->InsertEndChild(Pivot);
#pragma endregion

#pragma region LIFETIME
    tinyxml2::XMLElement* LifeTime = xmlDoc.NewElement("LifeTime");

    LifeTime->SetAttribute("Loop", pDesc->Mesh_Data.isLoop);
    LifeTime->SetAttribute("X", pDesc->Mesh_Data.vLifeTime.x);
    LifeTime->SetAttribute("Y", pDesc->Mesh_Data.vLifeTime.y);

    Effect->InsertEndChild(LifeTime);
#pragma endregion

#pragma region SHADER
    tinyxml2::XMLElement* PassIndex = xmlDoc.NewElement("PassIndex");

    PassIndex->SetAttribute("Index", pDesc->Mesh_Data.iPassIndex);

    Effect->InsertEndChild(PassIndex);

    tinyxml2::XMLElement* Diffuse = xmlDoc.NewElement("Diffuse");

    Diffuse->SetAttribute("ScrollSpeedU", pDesc->Mesh_Data.fDiffuseScrollSpeedU);
    Diffuse->SetAttribute("ScrollSpeedV", pDesc->Mesh_Data.fDiffuseScrollSpeedV);

    Effect->InsertEndChild(Diffuse);

    tinyxml2::XMLElement* Mask = xmlDoc.NewElement("Mask");

    Mask->SetAttribute("ScrollSpeedU", pDesc->Mesh_Data.fMaskScrollSpeedU);
    Mask->SetAttribute("ScrollSpeedV", pDesc->Mesh_Data.fMaskScrollSpeedV);

    Effect->InsertEndChild(Mask);
#pragma endregion

    root->InsertEndChild(Effect);

    if (tinyxml2::XML_SUCCESS != xmlDoc.SaveFile(pFilePath))
    {
        MSG_BOX("Failed to Save Effect data");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CEffect_Manager::Save_Ground(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID)
{
    tinyxml2::XMLDocument xmlDoc;

    if ((tinyxml2::XML_SUCCESS != xmlDoc.LoadFile(pFilePath)))
        return E_FAIL;

    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Data");

    EFFECT_GROUND_DESC* pDesc = static_cast<EFFECT_GROUND_DESC*>(pArg);

    tinyxml2::XMLElement* Effect = xmlDoc.NewElement("Effect");

    Effect->SetAttribute("id", iID);

    _uint itype = ENUM_TO_INT(eType);
    Effect->SetAttribute("type", itype);

#pragma region TEXTURE
    tinyxml2::XMLElement* Texture = xmlDoc.NewElement("Texture");

    Texture->SetAttribute("DiffuseTexture", m_pGameInstance->WstringToUtf8(pDesc->strDiffuseTexture).c_str());
    Texture->SetAttribute("MaskTexture", m_pGameInstance->WstringToUtf8(pDesc->strMaskTexture).c_str());
    Texture->SetAttribute("NoiseTexture", m_pGameInstance->WstringToUtf8(pDesc->strNoiseTexture).c_str());

    Effect->InsertEndChild(Texture);
#pragma endregion

#pragma region SCALE
    tinyxml2::XMLElement* Scale = xmlDoc.NewElement("Scale");

    _uint iLerp = ENUM_TO_INT(pDesc->Ground_Data.eLerpType);
    Scale->SetAttribute("Lerp", iLerp);

    Scale->SetAttribute("x", pDesc->Ground_Data.vSize.x);
    Scale->SetAttribute("y", pDesc->Ground_Data.vSize.y);

    Effect->InsertEndChild(Scale);
#pragma endregion

#pragma region ROTATION
    tinyxml2::XMLElement* Rotation = xmlDoc.NewElement("Rotation");

    Rotation->SetAttribute("Use", pDesc->Ground_Data.isRotation);

    Rotation->SetAttribute("StartX", pDesc->Ground_Data.vStartRotation.x);
    Rotation->SetAttribute("StartY", pDesc->Ground_Data.vStartRotation.y);
    Rotation->SetAttribute("StartZ", pDesc->Ground_Data.vStartRotation.z);

    Rotation->SetAttribute("EndX", pDesc->Ground_Data.vEndRotation.x);
    Rotation->SetAttribute("EndY", pDesc->Ground_Data.vEndRotation.y);
    Rotation->SetAttribute("EndZ", pDesc->Ground_Data.vEndRotation.z);

    Effect->InsertEndChild(Rotation);
#pragma endregion

#pragma region POSITION
    tinyxml2::XMLElement* Position = xmlDoc.NewElement("Position");

    Position->SetAttribute("X", pDesc->Ground_Data.vPosition.x);
    Position->SetAttribute("Y", pDesc->Ground_Data.vPosition.y);
    Position->SetAttribute("Z", pDesc->Ground_Data.vPosition.z);

    Effect->InsertEndChild(Position);
#pragma endregion

#pragma region PIVOT
    tinyxml2::XMLElement* Pivot = xmlDoc.NewElement("Pivot");

    Pivot->SetAttribute("Speed", pDesc->Ground_Data.fSpeed);

    Pivot->SetAttribute("X", pDesc->Ground_Data.vPivot.x);
    Pivot->SetAttribute("Y", pDesc->Ground_Data.vPivot.y);
    Pivot->SetAttribute("Z", pDesc->Ground_Data.vPivot.z);

    Effect->InsertEndChild(Pivot);
#pragma endregion

#pragma region LIFETIME
    tinyxml2::XMLElement* LifeTime = xmlDoc.NewElement("LifeTime");

    LifeTime->SetAttribute("X", pDesc->Ground_Data.vLifeTime.x);
    LifeTime->SetAttribute("Y", pDesc->Ground_Data.vLifeTime.y);

    Effect->InsertEndChild(LifeTime);
#pragma endregion

#pragma region SHADER
    tinyxml2::XMLElement* PassIndex = xmlDoc.NewElement("PassIndex");

    PassIndex->SetAttribute("Index", pDesc->Ground_Data.iPassIndex);

    Effect->InsertEndChild(PassIndex);

#pragma endregion

    root->InsertEndChild(Effect);

    if (tinyxml2::XML_SUCCESS != xmlDoc.SaveFile(pFilePath))
    {
        MSG_BOX("Failed to Save Effect data");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CEffect_Manager::Add_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_DiffuseFolder"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_MaskFolder"),
        TEXT("Com_MaskTexture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
        return E_FAIL;

    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_NoiseFolder"),
        TEXT("Com_NoiseTexture"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    return S_OK;
}

CEffect_Manager* CEffect_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Manager* pInstance = new CEffect_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CEffect_Manager");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CEffect_Manager::Clone(void* pArg)
{
    CGameObject* pInstance = new CEffect_Manager(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CEffect_Manager");
        return nullptr;
    }

    return pInstance;
}

void CEffect_Manager::Free()
{
    __super::Free();

    if(true == m_isCloned)
    {
        for (auto& EffecData : m_GroundEffectData)
            EffecData.clear();

        for (auto& EffecData : m_MeshEffect_Datas)
            EffecData.clear();

        for (auto& EffecData : m_ParticleEffect_Datas)
            EffecData.clear();

        for (auto& pEffect : m_MeshEffects)
            Safe_Release(pEffect);
        m_MeshEffects.clear();

        for (auto& pEffect : m_GroundEffects)
            Safe_Release(pEffect);
        m_GroundEffects.clear();

        for (auto& pEffect : m_ParticleEffects)
            Safe_Release(pEffect);
        m_ParticleEffects.clear();

        for (auto& pEffect : m_pActiveMeshEffects)
            Safe_Release(pEffect);
        m_pActiveMeshEffects.clear();

        for (auto& pEffect : m_pActiveGroundEffects)
            Safe_Release(pEffect);
        m_pActiveGroundEffects.clear();

        for (auto& pEffect : m_pActiveParticleEffects)
            Safe_Release(pEffect);
        m_pActiveParticleEffects.clear();

        Safe_Release(m_pTextureCom);
        Safe_Release(m_pMaskTextureCom);
        Safe_Release(m_pNoiseTextureCom);
    }
}
