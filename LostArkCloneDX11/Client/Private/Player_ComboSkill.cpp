#include "pch.h"
#include "Player_ComboSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_ComboSkill::CPlayer_ComboSkill()
	:CPlayer_State{}
{
}

HRESULT CPlayer_ComboSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_ComboSkill::Enter(void* pArg)
{
	COMBOSKILL_DESC* pSkill_Desc = static_cast<COMBOSKILL_DESC*>(pArg);

	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(pSkill_Desc->iSkillID);

	m_iKey = pSkill_Desc->iKey;
	m_iMaxCount = pSkill_Desc->iCount;
	m_iAnimationIndex = pSkill_Desc->iAnimationIndex;

	m_iCount = { 1 };
	m_fTimeAcc = 0.f;
	m_pPlayer->Set_Animation(m_iAnimationIndex++, false);
}

void CPlayer_ComboSkill::Update(_float fTimeDelta)
{
	// 피격 체크 추가

	m_pPlayer->Check_Navi();

	m_fTimeAcc += fTimeDelta;

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_iMaxCount <= m_iCount)
		{
			if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
			else
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		}
		else
		{
			if (m_pGameInstance->Get_KeyDown(m_iKey))
			{
				++m_iCount;
				m_pPlayer->Set_Animation(m_iAnimationIndex++, false);
				m_fTimeAcc = 0.f;
			}
		}
	}

	if (1.5f <= m_fTimeAcc)
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_ComboSkill::Exit()
{

}

CPlayer_ComboSkill* CPlayer_ComboSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_ComboSkill* pInstance = new CPlayer_ComboSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_ComboSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_ComboSkill::Free()
{
	__super::Free();
}