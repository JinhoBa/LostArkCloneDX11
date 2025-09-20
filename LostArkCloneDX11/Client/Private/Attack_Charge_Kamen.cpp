#include "pch.h"
#include "Attack_Charge_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Charge_Kamen::CAttack_Charge_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Charge_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CAttack_Charge_Kamen::Enter(void* pArg)
{
	ATTACK_KAMEN_DESC* pDesc = static_cast<ATTACK_KAMEN_DESC*>(pArg);

	m_iSkillID = 1;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	m_eState = STATE::START;
	m_pKamen->Set_Animation(36, false);
	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDesc.vOffset, m_pSkillDesc->HitBoxDesc.vExtends);
}

void CAttack_Charge_Kamen::Update(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CAttack_Charge_Kamen::STATE::START:
		if (m_pKamen->isAnimationFinish())
		{
			m_eState = STATE::LOOP;
			m_pKamen->Set_Animation(37, true);
		}
		break;

	case Client::CAttack_Charge_Kamen::STATE::LOOP:
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= 1.f)
		{
			m_eState = STATE::END;
			m_pKamen->Set_Animation(38, false);
		}
		break;

	case Client::CAttack_Charge_Kamen::STATE::END:
		Update_HitBox(fTimeDelta);
		if (m_pKamen->isAnimationFinish())
		{
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		}
		break;

	default:
		break;
	}
}

void CAttack_Charge_Kamen::Exit()
{

}

CAttack_Charge_Kamen* CAttack_Charge_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Charge_Kamen* pInstance = new CAttack_Charge_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Charge_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Charge_Kamen::Free()
{
	__super::Free();
}