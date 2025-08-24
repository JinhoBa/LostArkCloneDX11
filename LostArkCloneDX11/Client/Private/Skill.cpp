#include "Skill.h"

#include "GameManager.h"

CSkill::CSkill()
	: m_bUse{ true }, m_fCoolTime{0.f}
{
}

HRESULT CSkill::Initialize(_uint iSkillID)
{
	m_pSkill_Info = CGameManager::GetInstance()->Get_SkillInfo_Prt(iSkillID);

	if (nullptr == m_pSkill_Info)
		return E_FAIL;

	return S_OK;
}
void CSkill::Update(_float fTimeDelta)
{

	if (m_bUse)
	{
		m_fCoolTime -= fTimeDelta;

		if (0.f >= m_fCoolTime)
			m_bUse = true;
	}

}

_bool CSkill::Use_Skill()
{
	if (!m_bUse)
		return false;

	m_bUse = false;

	m_fCoolTime = m_pSkill_Info->fCoolTime;

	return true;
}

CSkill* CSkill::Create(_uint iSkillID)
{
	CSkill* pInstance = new CSkill();

	if (FAILED(pInstance->Initialize(iSkillID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CSkill");
		return nullptr;
	}

	return pInstance;
}

void CSkill::Free()
{
	__super::Free();
}