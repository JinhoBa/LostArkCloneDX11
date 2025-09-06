#include "pch.h"
#include "Skill_Manager.h"

#include "Skill.h"

CSkill_Manager::CSkill_Manager()
{
}

void CSkill_Manager::Set_PlayerInfoPrt(PLAYER_INFO* pPlayerInfo)
{
	for (auto& pSkill : m_Skills)
		pSkill->Set_PlayerInfo(pPlayerInfo);
}

const _float CSkill_Manager::Check_CoolTime(_uint iSkillID) const
{
	return m_Skills[iSkillID]->Get_CoolTime();
}

const _bool CSkill_Manager::Use_Skill(_uint iSkillID) const
{
	return m_Skills[iSkillID]->Use_Skill();
}

HRESULT	CSkill_Manager::Initialize()
{
	for (_uint i = 0; i < 18; i++)
	{
		CSkill* pSkill = CSkill::Create(i);

		if (nullptr == pSkill)
			return E_FAIL;

		m_Skills.push_back(pSkill);
	}

	return S_OK;
}
void CSkill_Manager::Update(_float fTimeDelta)
{
	for (auto& pSkill : m_Skills)
		pSkill->Update(fTimeDelta);
}

CSkill_Manager* CSkill_Manager::Create()
{
	CSkill_Manager* pInstance = new CSkill_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CSkill_Manager");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Manager::Free() 
{
	__super::Free();

	for (auto pSkill : m_Skills)
		Safe_Release(pSkill);
	m_Skills.clear();
}