#include "pch.h"
#include "Identity_Skill.h"

CIdentity_Skill::CIdentity_Skill()
	:m_fCoolTime{ 0.f }, m_fMaxfCoolTime{3.f}
{
}

HRESULT CIdentity_Skill::Initialize(PLAYER_INFO* pPlayerInfo)
{

	m_pPlayerInfo = pPlayerInfo;



	return S_OK;
}

void CIdentity_Skill::Update(_float fTimeDelta)
{

}


CIdentity_Skill* CIdentity_Skill::Create(PLAYER_INFO* pPlayerInfo)
{
	CIdentity_Skill* pInstance = new CIdentity_Skill();

	if (FAILED(pInstance->Initialize(pPlayerInfo)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create");
		return nullptr;
	}

	return pInstance;
}
void CIdentity_Skill::Free()
{
	__super::Free();
}
