#include "pch.h"
#include "Attack_Spin_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Spin_Kamen::CAttack_Spin_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Spin_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CAttack_Spin_Kamen::Enter(void* pArg)
{
	m_eHitboxType = COLLIDER::OBB;
	m_iSkillID = 2;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	_float fDot = XMVectorGetX(XMVector3Dot(
		m_pKamen->Get_Transform()->Get_State(STATE::RIGHT), m_pPlayerTransform->Get_Position()));

	if(-0.5f >= fDot)
		m_pKamen->Set_Animation(4, false);
	else if(0.5f <= fDot)
		m_pKamen->Set_Animation(3, false);
	else
		m_pKamen->Set_Animation(2, false);

	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOffset, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vExtends);
}

void CAttack_Spin_Kamen::Update(_float fTimeDelta)
{
	Update_HitBox(fTimeDelta);

	if (m_pKamen->isAnimationFinish())
		m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
}

void CAttack_Spin_Kamen::Exit()
{

}

CAttack_Spin_Kamen* CAttack_Spin_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Spin_Kamen* pInstance = new CAttack_Spin_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Spin_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Spin_Kamen::Free()
{
	__super::Free();
}