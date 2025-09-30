#include "pch.h"
#include "Player_Dialogue.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Dialogue::CPlayer_Dialogue()
	:CState_Player{}
{
}

HRESULT CPlayer_Dialogue::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Dialogue::Enter(void* pArg)
{
	PLAYER_DIALOGUE_DESC* pDesc = static_cast<PLAYER_DIALOGUE_DESC*>(pArg);

	_float3 vTargetPosition = pDesc->vTargetPosition;
	_float3 vPosition = pDesc->vPosition;

	CTransform* pTransform = m_pPlayer->Get_Transform();

	XMStoreFloat4(&m_vPrePosition, pTransform->Get_Position());

	pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));
	
	pTransform->TurnTo(XMVectorSetW(XMLoadFloat3(&vTargetPosition), 1.f));

	if (STANCE::FLURRY == *m_pPlayerStance)
	{
		m_pPlayer->Set_Animation(35, true);
	}
	else
	{
		m_pPlayer->Set_Animation(36, true);
	}
}

void CPlayer_Dialogue::Update(_float fTimeDelta)
{
	
}

void CPlayer_Dialogue::Exit()
{
	m_pPlayer->Get_Transform()->Set_State(STATE::POSITION, XMLoadFloat4(&m_vPrePosition));
}

CPlayer_Dialogue* CPlayer_Dialogue::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Dialogue* pInstance = new CPlayer_Dialogue();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Dialogue");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Dialogue::Free()
{
	__super::Free();
}