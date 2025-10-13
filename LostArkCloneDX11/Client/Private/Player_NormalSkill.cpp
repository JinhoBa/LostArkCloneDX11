#include "pch.h"
#include "Player_NormalSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Effect_Ground.h"

CPlayer_NormalSkill::CPlayer_NormalSkill()
	:CSkill_Player{}
{
}

HRESULT CPlayer_NormalSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_NormalSkill::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pPlayer->TurnToCursor();

	NORMALSKILL_DESC* m_pSkill_Desc = static_cast<NORMALSKILL_DESC*>(pArg);

	m_iSkillID = m_pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_pSkill_Desc->iSkillID);

	if(10 == m_iSkillID)
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop, 0.f);
	else
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop);

	/* HitBox */
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends);

	CEffect_Ground::EffectGroundDesc Effect_Desc = {};

	XMStoreFloat3(&Effect_Desc.vPosition, m_pPlayer->Get_Transform()->Get_Position());

	m_pGameManager->Add_Effect(EFFECT::GROUND, 0, &Effect_Desc);
}

void CPlayer_NormalSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	Update_Hitbox(fTimeDelta);

	// 피격 체크 추가

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_NormalSkill::Exit()
{
	m_pPlayer->Set_SkillID(99);
}

CPlayer_NormalSkill* CPlayer_NormalSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_NormalSkill* pInstance = new CPlayer_NormalSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_NormalSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_NormalSkill::Free()
{
	__super::Free();
}