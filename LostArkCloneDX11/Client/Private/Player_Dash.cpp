#include "pch.h"
#include "Player_Dash.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Weapon_Player.h"
#include "Body_Player.h"

CPlayer_Dash::CPlayer_Dash()
	:CState_Player{}
{
}

HRESULT CPlayer_Dash::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Dash::Enter(void* pArg)
{
	if(STANCE::FLURRY ==  *m_pPlayerStance)
		m_pPlayer->Set_Animation(127, false, 0.f);
	else
		m_pPlayer->Set_Animation(128, false, 0.f);

	m_bChangeStance = false;
	m_pPlayer->Set_SkillID(16);

	dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();
}

void CPlayer_Dash::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	if (m_pGameInstance->Get_KeyDown(DIK_Z) && false == m_bChangeStance)
	{
		m_pPlayer->Change_Stance();
		m_bChangeStance = true;
	}

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_Dash::Exit()
{
	dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

	m_pPlayer->Set_SkillID(99);
}

CPlayer_Dash* CPlayer_Dash::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Dash* pInstance = new CPlayer_Dash();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Dash");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Dash::Free()
{
	__super::Free();
}