#include "pch.h"
#include "Player_CutScene.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_CutScene::CPlayer_CutScene()
	:CState_Player{}
{
}

HRESULT CPlayer_CutScene::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_CutScene::Enter(void* pArg)
{
	if (STANCE::FLURRY == *m_pPlayerStance)
	{
		m_pPlayer->Set_Animation(35, true);
	}
	else
	{
		m_pPlayer->Set_Animation(36, true);
	}

	m_pPlayer->Toggle_PartObject(TEXT("HPBar_Player"));
	Toggle_HUD();
}

void CPlayer_CutScene::Update(_float fTimeDelta)
{
	
}

void CPlayer_CutScene::Exit()
{
	m_pPlayer->Toggle_PartObject(TEXT("HPBar_Player"));
	Toggle_HUD();
}

CPlayer_CutScene* CPlayer_CutScene::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_CutScene* pInstance = new CPlayer_CutScene();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_CutScene");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_CutScene::Free()
{
	__super::Free();
}