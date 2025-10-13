#include "pch.h"
#include "Effect_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"
#include "Monster.h"
#include "Kamen.h"
#include "Player.h"
#include "Effect_Ground.h"

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
    for (_uint i = 0; i < 30; i++)
    {
        m_GroundEffects.push_back(
            dynamic_cast<CEffect_Ground*>(m_pGameInstance->Clone_Prototype(
                PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Ground")))
        );
    }

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
            m_GroundEffects.push_back(*iter);
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

void CEffect_Manager::Add_Effects(EFFECT eType, _uint iEffectID, void* pArg)
{
   /* m_GroundEffects.front()->Start(pArg);

    m_pActiveEffects.push_back(m_GroundEffects.front());

    m_GroundEffects.pop_front();*/
}

HRESULT CEffect_Manager::Load_Data(const _char* pFilePath)
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
        EFFECT_GROUND_DESC Effect_Desc = {};
        Effect->QueryUnsignedAttribute("id", &Effect_Desc.iID);

        /*Effect Texture */
        tinyxml2::XMLElement* Texture = Effect->FirstChildElement("Texture");

        Texture->QueryUnsignedAttribute("Base", &Effect_Desc.iBaseIndex);
        Texture->QueryUnsignedAttribute("Mask", &Effect_Desc.iMaskIndex);
        Texture->QueryUnsignedAttribute("Noise", &Effect_Desc.iNoiseIndex);

        /*Effect Size */
        tinyxml2::XMLElement* Size = Effect->FirstChildElement("Size");

       /* Size->QueryUnsignedAttribute("Lerp", &Effect_Desc.eLerpType);
        Size->QueryUnsignedAttribute("Mask", &Effect_Desc.iMaskIndex);
        Size->QueryUnsignedAttribute("Noise", &Effect_Desc.iNoiseIndex);*/
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

        for (auto& pEffect : m_pActiveEffects)
            Safe_Release(pEffect);
        m_pActiveEffects.clear();
    }
}
