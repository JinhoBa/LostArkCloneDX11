#include "pch.h"
#include "Player_ComboSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_ComboSkill::CPlayer_ComboSkill()
	:CSkill_Player{}
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
	__super::Enter(pArg);

	COMBOSKILL_DESC* pSkill_Desc = static_cast<COMBOSKILL_DESC*>(pArg);

	m_iSkillID = pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(pSkill_Desc->iSkillID);

	m_iKey = pSkill_Desc->iKey;
	m_iMaxCount = pSkill_Desc->iCount;
	m_iAnimationIndex = pSkill_Desc->iAnimationIndex;

	m_isInputKey = false;

	m_iCount = { 1 };

	m_pPlayer->Set_Animation(m_iAnimationIndex++, false);
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends);
}

void CPlayer_ComboSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	if (m_iAttackCount >= m_pSkillInfo->iNumAttack)
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}

	__super::Update_Hitbox(fTimeDelta);

	if (m_pPlayer->isAnimationFinish())
	{
		if (true == m_isInputKey)
		{
			m_isInputKey = false;
			m_pPlayer->Set_Animation(m_iAnimationIndex++, false);
		}
		else
		{
			if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
			else
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		}
	}

	if (m_pGameInstance->Get_KeyDown(m_iKey))
		m_isInputKey = true;
	
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