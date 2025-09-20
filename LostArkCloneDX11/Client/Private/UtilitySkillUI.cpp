#include "pch.h"
#include "UtilitySkillUI.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "UtilitySkillSlot.h"

CUtilitySkillUI::CUtilitySkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHUD{ pDevice, pContext }
{
}

CUtilitySkillUI::CUtilitySkillUI(const CUtilitySkillUI& Prototype)
	: CHUD{ Prototype }
{
}

HRESULT CUtilitySkillUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUtilitySkillUI::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Slots()))
		return E_FAIL;


	return S_OK;
}

void CUtilitySkillUI::Priority_Update(_float fTimeDelta)
{
	for (auto& pSkill : m_UtilitySkills)
	{
		pSkill->Priority_Update(fTimeDelta);
	}

	m_iNumSkill = 0;

	for(auto& pSkill : m_UtilitySkills)
	{
		if (pSkill->isUsed())
			++m_iNumSkill;
	}

}

void CUtilitySkillUI::Update(_float fTimeDelta)
{
	for (auto& pSkill : m_UtilitySkills)
	{
		pSkill->Update(fTimeDelta);
	}
}

void CUtilitySkillUI::Late_Update(_float fTimeDelta)
{
	for (auto& pSkill : m_UtilitySkills)
	{
		pSkill->Late_Update(fTimeDelta);
	}

	__super::Late_Update(fTimeDelta);
}

HRESULT CUtilitySkillUI::Render()
{
	if (0 == m_iNumSkill)
		return S_OK;
	else if (1 == m_iNumSkill)
	{
		for (auto& pSkill : m_UtilitySkills)
		{
			if (pSkill->isUsed())
				pSkill->Render();
		}
	}
	else
	{
		m_fX -= 40.f;
		Update_Position();
		m_UtilitySkills[0]->Render();

		m_fX += 80.f;
		Update_Position();
		m_UtilitySkills[1]->Render();

		m_fX -= 40.f;
	}

	return S_OK;
}


HRESULT CUtilitySkillUI::Add_Slots()
{
	CUtilitySkillSlot::UTILITYSLOT_DESC Desc = {};

	Desc.pParent_TransformCom = m_pTransformCom;
	Desc.fX = 0.f;
	Desc.fY = -80.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 40.f;
	Desc.fSizeY = 40.f;

	Desc.iSlotID = 16;
	Desc.iTextureIndex = 0;
	Desc.iSubTextureIndex = 1;

	CUtilitySkillSlot* pSlot = dynamic_cast<CUtilitySkillSlot*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UtilitySkillSlot"), &Desc));

	if (nullptr == pSlot)
		return E_FAIL;

	m_UtilitySkills.push_back(pSlot);

	Desc.iSlotID = 17;
	Desc.iTextureIndex = 2;
	Desc.iSubTextureIndex = 2;

	pSlot = dynamic_cast<CUtilitySkillSlot*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UtilitySkillSlot"), &Desc));

	if (nullptr == pSlot)
		return E_FAIL;

	m_UtilitySkills.push_back(pSlot);

	return S_OK;
}

CUtilitySkillUI* CUtilitySkillUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUtilitySkillUI* pInstance = new CUtilitySkillUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CUtilitySkillUI");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUtilitySkillUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CUtilitySkillUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CUtilitySkillUI");
		return nullptr;
	}

	return pInstance;
}

void CUtilitySkillUI::Free()
{
	__super::Free();

	for (auto& pSlot : m_UtilitySkills)
		Safe_Release(pSlot);
	m_UtilitySkills.clear();
}
