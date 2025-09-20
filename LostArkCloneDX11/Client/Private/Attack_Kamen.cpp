#include "pch.h"
#include "Attack_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Kamen::CAttack_Kamen()
	:CState_Kamen{}
{
}

HRESULT CAttack_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (nullptr == m_pPlayerTransform)
		return E_FAIL;

	Safe_AddRef(m_pPlayerTransform);

	return S_OK;
}

void CAttack_Kamen::Enter(void* pArg)
{
}

void CAttack_Kamen::Update(_float fTimeDelta)
{

}

void CAttack_Kamen::Exit()
{

}

void CAttack_Kamen::Update_HitBox(_float fTimeDelta)
{
	if (m_pKamen->Get_TrackPositon() <= m_SkillDesc.HitBoxDesc.fStartTime)
		m_isActiveHitBox = true;

	if (m_iAttackCount <= m_SkillDesc.iNumAttack)
	{
		if (m_isActiveHitBox)
		{
			m_fTimeAcc += fTimeDelta;
			m_pKamen->Update_HitBox(m_iSkillID, m_iAttackCount);

			if (m_SkillDesc.HitBoxDesc.fDuration <= m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				m_isActiveHitBox = false;
				++m_iAttackCount;
			}
		}
		else
		{
			m_fTimeAcc += fTimeDelta;
			if (m_SkillDesc.HitBoxDesc.fInterval <= m_fTimeAcc)
			{
				m_isActiveHitBox = true;
				m_fTimeAcc = 0.f;
			}
		}
	}
}

void CAttack_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransform);
}