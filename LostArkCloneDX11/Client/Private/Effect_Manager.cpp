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
    for (_uint i = 0; i < 20; i++)
    {
        m_GroundEffects.push_back(
            dynamic_cast<CEffect_Ground*>(m_pGameInstance->Clone_Prototype(
                PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Ground")))
        );
    }

    for (_uint i = 0; i < 15; i++)
    {
        m_MeshEffects.push_back(
            dynamic_cast<CEffect_Mesh*>(m_pGameInstance->Clone_Prototype(
                PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Mesh")))
        );
    }

    if (FAILED(Load_Mesh_Data("../Bin/Resources/Data/Effect/Player_Effects.xml")))
        return E_FAIL;

    return S_OK;
}

void CEffect_Manager::Priority_Update(_float fTimeDelta)
{
    for (auto& pEffect : m_pActiveEffects)
    {
        pEffect->Priority_Update(fTimeDelta);
    }
}

void CEffect_Manager::Update(_float fTimeDelta)
{
    for (auto& pEffect : m_pActiveEffects)
    {
        pEffect->Update(fTimeDelta);
    }
}

void CEffect_Manager::Late_Update(_float fTimeDelta)
{
    auto iter = m_pActiveEffects.begin();
    for (;iter != m_pActiveEffects.end(); )
    {
        if ((*iter)->isDead())
        {
            (*iter)->Reset();
            m_MeshEffects.push_back(*iter);
            iter = m_pActiveEffects.erase(iter);
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

void CEffect_Manager::Add_Effects(EFFECT eType, _uint iEffectID, const _float4x4* pWorldMatirx, void* pArg)
{
    switch (eType)
    {
    case Client::EFFECT::BILLBORD:
        break;

    case Client::EFFECT::TRAIL:
        break;

    case Client::EFFECT::GROUND:
        m_GroundEffects.front()->Start(pWorldMatirx, pArg);

        m_pActiveEffects.push_back(m_GroundEffects.front());

        m_GroundEffects.pop_front();
        break;

    case Client::EFFECT::PARTICLE:
        break;

    case Client::EFFECT::MESH:
        m_MeshEffects.front()->Start(pWorldMatirx, &(m_MeshEffect_Datas[iEffectID]));

        m_pActiveEffects.push_back(m_MeshEffects.front());

        m_MeshEffects.pop_front();
        break;
    }
        
}

HRESULT CEffect_Manager::Load_Mesh_Data(const _char* pFilePath)
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
        tinyxml2::XMLElement* Noise = Effect->FirstChildElement("Noise");

        Noise->QueryFloatAttribute("Strength", &Desc.Mesh_Data.fNoiseStrength);


        tinyxml2::XMLElement* Diffuse = Effect->FirstChildElement("Diffuse");

        Diffuse->QueryFloatAttribute("ScrollSpeedU", &Desc.Mesh_Data.fDiffuseScrollSpeedU);
        Diffuse->QueryFloatAttribute("ScrollSpeedV", &Desc.Mesh_Data.fDiffuseScrollSpeedV);


        tinyxml2::XMLElement* Mask = Effect->FirstChildElement("Mask");

        Mask->QueryFloatAttribute("ScrollSpeedU", &Desc.Mesh_Data.fMaskScrollSpeedU);
        Mask->QueryFloatAttribute("ScrollSpeedV", &Desc.Mesh_Data.fMaskScrollSpeedV);

#pragma endregion

        m_MeshEffect_Datas.push_back(Desc);
    }


    return S_OK;
}

HRESULT CEffect_Manager::Save_Effect(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID)
{
    if (EFFECT::MESH == eType)
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
        tinyxml2::XMLElement* Noise = xmlDoc.NewElement("Noise");

        Noise->SetAttribute("Strength", pDesc->Mesh_Data.fNoiseStrength);

        Effect->InsertEndChild(Noise);

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
       
    }

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
        for (auto& pEffect : m_GroundEffects)
            Safe_Release(pEffect);
        m_GroundEffects.clear();

        for (auto& pEffect : m_MeshEffects)
            Safe_Release(pEffect);
        m_MeshEffects.clear();

        for (auto& pEffect : m_pActiveEffects)
            Safe_Release(pEffect);
        m_pActiveEffects.clear();
    }
}
