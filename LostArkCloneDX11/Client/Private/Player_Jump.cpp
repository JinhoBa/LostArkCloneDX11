#include "pch.h"
#include "Player_Jump.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"

CPlayer_Jump::CPlayer_Jump()
	:CPlayer_State{}
{
}

HRESULT CPlayer_Jump::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Jump::Enter(void* pArg)
{
	m_vTargetPosition = _float4(35.f, 0.1f, 32.f, 1.f);
	m_pPlayer->Set_Animation(1, true, 0.1f);

	m_fHeight = XMVectorGetY(m_pPlayer->Get_Transform()->Get_Position());
	m_fRation = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayer_Jump::Update(_float fTimeDelta)
{
	_vector vPosition = m_pPlayer->Get_Transform()->Get_Position();

	m_fTimeAcc += fTimeDelta;
	m_fRation += m_fTimeAcc * 0.02f;
	
	_float fsin = sin(XMConvertToRadians(m_fRation * 270.f));
	 _float fHegiht = m_fHeight + m_fHeight * fsin;


	if (m_pPlayer->Jump(XMLoadFloat4(&m_vTargetPosition), fTimeDelta, m_fRation, fHegiht))
	{
		m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_Jump::Exit()
{

}

CPlayer_Jump* CPlayer_Jump::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_Jump* pInstance = new CPlayer_Jump();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_Jump");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Jump::Free()
{
	__super::Free();
}