#include "pch.h"
#include "Clash_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"
#include "Player.h"
#include "Camera_Clash.h"

CClash_Kamen::CClash_Kamen()
	:CState_Kamen{}
{
}

HRESULT CClash_Kamen::Initilize(STATE_KAMEN_DESC* pDesc, CTransform* pPlayerTransform)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	m_pPlayerTransform = pPlayerTransform;

	Safe_AddRef(m_pPlayerTransform);

	return S_OK;
}

void CClash_Kamen::Enter(void* pArg)
{
	m_eState = STATE::IDLE;
	
	m_pKamen->Set_Animation(6, false);
}

void CClash_Kamen::Update(_float fTimeDelta)
{
	/*if (2.f >= XMVectorGetX(XMVector3Length(m_pKamen->Get_Transform()->Get_Position() - m_pPlayerTransform->Get_Position())))
	{
		if (m_pGameInstance->Get_KeyDown(DIK_G))
		{

		}
	}*/

	switch (m_eState)
	{
	case Client::CClash_Kamen::STATE::IDLE:
		if (m_pGameInstance->Get_KeyDown(DIK_G))
		{
			m_eState = STATE::START;
			/*test*/
			//m_pKamen->Set_Animation(7, true);

			m_pKamen->Set_Animation(7, false);
			m_pKamen->Get_Transform()->TurnTo(m_pPlayerTransform->Get_Position());

			m_pPlayerTransform->TurnTo(m_pKamen->Get_Transform()->Get_Position());

			dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(
				ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Start_Clash();

			dynamic_cast<CCamera_Clash*>(m_pGameInstance->Find_Camera(TEXT("Camera_Clash"))
				)->Set_Position(m_pPlayerTransform->Get_Position());

			m_pGameInstance->Bind_Camera(TEXT("Camera_Clash"));
		}
		break;

	case Client::CClash_Kamen::STATE::START:

		if (m_pKamen->isAnimationFinish())
		{
			m_pKamen->Set_Animation(8, true);
			m_eState = STATE::LOOP;
		}
		break;

	case Client::CClash_Kamen::STATE::LOOP:
		_uint iResult = m_pGameManager->Update_Clash(fTimeDelta);

		if (1 == iResult)
		{
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::CRITICAL), nullptr);
			m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"), false, 1.5f);
		}
		else if (2 == iResult)
		{
			m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::IDLE), nullptr);
			m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"), false, 1.5f);
		}

		break;
	}


	//m_pStateMachine->Change_State(m_pKamen->Get_State(CKamen::KAMENSTATE::ATTACK_NORMAL), nullptr);
			

}

void CClash_Kamen::Exit()
{
}

CClash_Kamen* CClash_Kamen::Create(STATE_KAMEN_DESC* pDesc, CTransform* pPlayerTransform)
{
	CClash_Kamen* pInstance = new CClash_Kamen();

	if (FAILED(pInstance->Initilize(pDesc, pPlayerTransform)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CClash_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CClash_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransform);
}