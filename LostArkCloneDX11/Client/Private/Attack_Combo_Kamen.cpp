#include "pch.h"
#include "Attack_Combo_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CAttack_Combo_Kamen::CAttack_Combo_Kamen()
	:CAttack_Kamen{}
{
}

HRESULT CAttack_Combo_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	m_KeyFrames[0] = 15;
	m_KeyFrames[1] = 35;
	m_KeyFrames[2] = 40;


	return S_OK;
}

void CAttack_Combo_Kamen::Enter(void* pArg)
{
	m_eHitboxType = COLLIDER::SPHERE;
	m_iSkillID = 3;
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;

	for (auto& Trigger : m_Triggers)
		Trigger = false;

	m_pSkillDesc = m_pGameManager->Get_KamenData(ENUM_TO_INT(*m_pPhase), m_iSkillID);

	m_eState = STATE::READY;

	m_pKamen->Set_Animation(192, false);
	m_pKamen->Set_HitBox(m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOffset, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vExtends, m_pSkillDesc->HitBoxDescs[m_iAttackCount].vOrientation);

	m_iEffectID = 4;
	Ready_EffectEvents();
}

void CAttack_Combo_Kamen::Update(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CAttack_Combo_Kamen::STATE::READY:
		if (m_pKamen->isAnimationFinish())
		{
			m_eState = STATE::START;
			m_pKamen->Set_Animation(179, false);
			m_pGameInstance->Play_Sound(L"Kamen1_Skill_2_3.wav", CHANNELID::EFFECT, 0.7f);
		}
		break;

	case Client::CAttack_Combo_Kamen::STATE::START:
		if (m_pKamen->isAnimationFinish())
		{
			m_eState = STATE::LOOP;
			m_pKamen->Set_Animation(180, false);
		}
		break;

	case Client::CAttack_Combo_Kamen::STATE::LOOP:
		m_fTimeAcc += fTimeDelta;
		Update_EffectTrack();
		if (m_fTimeAcc >= 1.f)
		{
			m_EffectEvents.clear();
			m_iEffectID = 5;
			Ready_EffectEvents();

			m_eState = STATE::ATTACK;
			m_pKamen->Set_Animation(181, false);
		}
		break;

	case Client::CAttack_Combo_Kamen::STATE::ATTACK:
		Update_EffectTrack();
		//Update_HitRange(fTimeDelta);

		if (m_pKamen->isAnimationFinish())
		{
			m_eState = STATE::END;
			m_pKamen->Set_Animation(190, false);
		}
		break;

	case Client::CAttack_Combo_Kamen::STATE::END:
		if (m_pKamen->isAnimationFinish())
		{
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
		}
		break;

	default:
		break;
	}
	
}

void CAttack_Combo_Kamen::Exit()
{
	m_EffectEvents.clear();
}

void CAttack_Combo_Kamen::Update_HitRange(_float fTimeDelta)
{
	for (_uint i = 0; i < 3; i++)
	{
		if (false == m_Triggers[i] && m_KeyFrames[i] <= m_pKamen->Get_TrackPositon())
		{
			m_Triggers[i] = true;

			_float fDistance = XMVectorGetX(XMVector3Length(m_pPlayerTransform->Get_Position() - m_pKamen->Get_Transform()->Get_Position()));

			/* 범위 확인 */
			if(0 == i && 3.f >= fDistance )
				m_pKamen->HitBox_Event(m_iSkillID, i);
			else if ((1 == i || 2 == i) && 3.f <= fDistance)
				m_pKamen->HitBox_Event(m_iSkillID, i);
		}
	}
}

CAttack_Combo_Kamen* CAttack_Combo_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CAttack_Combo_Kamen* pInstance = new CAttack_Combo_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAttack_Combo_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CAttack_Combo_Kamen::Free()
{
	__super::Free();
}