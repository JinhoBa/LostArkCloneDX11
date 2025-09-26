#include "pch.h"
#include "Clash_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

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
	
	m_pKamen->Set_Animation(6, false);
	
}

void CClash_Kamen::Update(_float fTimeDelta)
{
	if (2.f >= XMVectorGetX(XMVector3Length(m_pKamen->Get_Transform()->Get_Position() - m_pPlayerTransform->Get_Position())))
	{
		if (m_pGameInstance->Get_KeyDown(DIK_G))
		{

		}
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
		MSG_BOX("Failed to Create : CIdel_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CClash_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransform);
}