#include "pch.h"
#include "Player_Idle.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Player_Move.h"
#include "Player_NormalSkill.h"
#include "Player_ChargeSkill.h"
#include "Player_ComboSkill.h"
#include "Player_ChangeStance.h"

CPlayer_Idle::CPlayer_Idle()
	:CPlayer_State{}
{
}

HRESULT CPlayer_Idle::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if(FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Idle::Enter(void* pArg)
{
	if (STANCE::FLURRY == *m_pPlayerStance)
	{
		m_pPlayer->Set_Animation(35, true);
	}
	else
	{
		m_pPlayer->Set_Animation(36, true);
	}
}

void CPlayer_Idle::Update(_float fTimeDelta)
{
	if(__super::Check_Dash())
	{
		m_pPlayer->TurnToCursor();
		m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::DASH), nullptr);
		return;
	}

	if (m_pGameInstance->Get_KeyDown(DIK_Z))
	{
		m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::CHANGE_STANCE), nullptr);
		return;
	}

	_bool isUseSkill = { false };
	_bool bLoop = { false };
	_uint iSkillID = {};
	_uint iAnimationIndex = {};
	_uint iKey = {};
	_uint iCount = {};

	if (m_pGameInstance->Get_KeyDown(DIK_V))
	{
		iSkillID = 14;
		iAnimationIndex = 115;
		isUseSkill = m_pGameManager->Use_Skill(iSkillID);
	}


	if (STANCE::FLURRY == *m_pPlayerStance)
	{
	
		if (m_pGameInstance->Get_KeyDown(DIK_W))
		{
			iSkillID = 1;
			iAnimationIndex = 174;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_E))
		{
			iSkillID = 2;
			iAnimationIndex = 137;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
			
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_R))
		{
			iSkillID = 3;
			iAnimationIndex = 70;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_A))
		{
			iSkillID = 4;
			iAnimationIndex = 48;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_S))
		{
			iSkillID = 5;
			iAnimationIndex = 151;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_D))
		{
			iSkillID = 6;
			iAnimationIndex = 30;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
			
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_F))
		{
			iSkillID = 7;
			iAnimationIndex = 61;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
	}
	else
	{
		if (m_pGameInstance->Get_KeyDown(DIK_Q))
		{
			iSkillID = 8;
			iAnimationIndex = 164;
			iCount = 2;
			iKey = DIK_Q;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_W))
		{
			iSkillID = 9;
			iAnimationIndex = 58;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_R))
		{
			iSkillID = 10;
			iAnimationIndex = 109;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_S))
		{
			iSkillID = 11;
			iAnimationIndex = 195;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_D))
		{
			iSkillID = 12;
			iKey = DIK_D;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_F))
		{
			iSkillID = 13;
			iKey = DIK_F;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
	}

	if (true == isUseSkill)
	{
		SKILL_TYPE eType = m_pGameManager->Get_SkillInfo_Prt(iSkillID)->eSkilltype;

		if(SKILL_TYPE::SKILL_NORAML == eType)
		{
			CPlayer_NormalSkill::NORMALSKILL_DESC Desc = {};

			Desc.bLoop = bLoop;
			Desc.iAnimationIndex = iAnimationIndex;
			Desc.iSkillID = iSkillID;

			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::NORMAL_SKILL), &Desc);
		}
		else if(SKILL_TYPE::SKILL_CHARGE == eType)
		{
			CPlayer_ChargeSkill::CHARGESTATE_DESC Desc = {};

			Desc.iKey = iKey;
			Desc.iSkillID = iSkillID;

			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::CHARGE_SKILL), &Desc);
		}
		else if(SKILL_TYPE::SKILL_COMBO == eType)
		{
			CPlayer_ComboSkill::COMBOSKILL_DESC Desc = {};

			Desc.iKey = iKey;
			Desc.iSkillID = iSkillID;
			Desc.iAnimationIndex = iAnimationIndex;
			Desc.iCount = iCount;
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::COMBO_SKILL), &Desc);
		}
		else
		{
			CPlayer_NormalSkill::NORMALSKILL_DESC Desc = {};

			Desc.bLoop = bLoop;
			Desc.iAnimationIndex = iAnimationIndex;
			Desc.iSkillID = iSkillID;

			m_pPlayer->TurnToCursor();
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::NORMAL_SKILL), &Desc);
		}
	}
	else
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
		{
			if(true == m_pPlayer->Move(fTimeDelta))
				m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		}
			
	}
}

void CPlayer_Idle::Exit()
{

}

CPlayer_Idle* CPlayer_Idle::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Idle* pInstance = new CPlayer_Idle();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Idle");
		return nullptr;
	}
	
	return pInstance;
}

void CPlayer_Idle::Free()
{
	__super::Free();
}