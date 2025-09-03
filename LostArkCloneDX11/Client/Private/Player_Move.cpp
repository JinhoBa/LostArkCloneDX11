#include "pch.h"
#include "Player_Move.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Player_NormalSkill.h"

CPlayer_Move::CPlayer_Move()
	:CPlayer_State{}
{
}

HRESULT CPlayer_Move::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Move::Enter(void* pArg)
{
	if (nullptr == pArg)
	{
		m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
		return;
	}

	PLAYERMOVE_DESC* pDesc = static_cast<PLAYERMOVE_DESC*>(pArg);

	memcpy(&m_vPinkingPosition, &pDesc->vPickingPosition, sizeof(_float3));

	if (STANCE::FLURRY == *m_pPlayerStance)
	{
		m_pPlayer->Set_Animation(45, true);
	}
	else
	{
		m_pPlayer->Set_Animation(46, true);
	}
}

void CPlayer_Move::Update(_float fTimeDelta)
{
	_bool isUseSkill = { false };
	_bool bLoop = { false };
	_uint iSkillID = {};
	_uint iAnimationIndex = {};

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
			iAnimationIndex = 17;
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
			iAnimationIndex = 126;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
		else if (m_pGameInstance->Get_KeyDown(DIK_F))
		{
			iSkillID = 13;
			iAnimationIndex = 133;
			isUseSkill = m_pGameManager->Use_Skill(iSkillID);
		}
	}

	if (true == isUseSkill)
	{
		CPlayer_NormalSkill::NORMALSKILL_DESC Desc = {};

		Desc.bLoop = bLoop;
		Desc.iAnimationIndex = iAnimationIndex;
		Desc.iSkillID = iSkillID;

		m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::NORMAL_SKILL), &Desc);
		return;
	}
	else
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
		{
			_float3* pPickingPos = m_pGameManager->Get_PickingPos();

			if (nullptr != pPickingPos)
				memcpy(&m_vPinkingPosition, pPickingPos, sizeof(_float3));
		}

		if (false == m_pPlayer->Move(fTimeDelta, XMVectorSetW(XMLoadFloat3(&m_vPinkingPosition), 1.f)))
		{
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
			return;
		}
	}
}

void CPlayer_Move::Exit()
{

}

CPlayer_Move* CPlayer_Move::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Move* pInstance = new CPlayer_Move();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Move");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Move::Free()
{
	__super::Free();
}