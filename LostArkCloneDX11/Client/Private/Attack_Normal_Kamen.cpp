#include "pch.h"
#include "Attack_Normal_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Normal_Kamen::CAttack_Normal_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Normal_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CAttack_Normal_Kamen::Enter(void* pArg)
{
	ATTACK_KAMEN_DESC* pDesc = static_cast<ATTACK_KAMEN_DESC*>(pArg);

	m_iSkillID = pDesc->iSkillID;

	memcpy(&m_SkillDesc, &m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase))[m_iSkillID], sizeof(KAMEN_SKILL_DESC));

	m_pKamen->Set_Animation(m_SkillDesc.iAnimationIndexStart, false);
}

void CAttack_Normal_Kamen::Update(_float fTimeDelta)
{
	if (m_pKamen->isAnimationFinish())
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);

}

void CAttack_Normal_Kamen::Exit()
{

}

CAttack_Normal_Kamen* CAttack_Normal_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Normal_Kamen* pInstance = new CAttack_Normal_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Normal_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Normal_Kamen::Free()
{
	__super::Free();
}