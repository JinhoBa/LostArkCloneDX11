#include "pch.h"
#include "Intro_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "StateMachine.h"
#include "Kamen.h"

CIntro_Kamen::CIntro_Kamen()
	:CState_Kamen{}
{
}

HRESULT CIntro_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (FAILED(__super::Initilize(pDesc)))
		return E_FAIL;


	return S_OK;
}

void CIntro_Kamen::Enter(void* pArg)
{
	m_fTimeAcc = 0.f;
	
	m_pKamen->Set_Animation(193, true);

}

void CIntro_Kamen::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (false == m_bActive && 3.f <= m_fTimeAcc)
	{
		if(0.5f <= m_pGameInstance->Random(0.f, 1.f))
			m_pKamen->Set_Animation(182, false);
		else
			m_pKamen->Set_Animation(183, false);

		m_bActive = true;
	}
	
	if(true == m_bActive && m_pKamen->isAnimationFinish())
		m_pKamen->Set_Animation(193, true);

}

void CIntro_Kamen::Exit()
{

}

CIntro_Kamen* CIntro_Kamen::Create(STATE_KAMEN_DESC* pDesc)
{
	CIntro_Kamen* pInstance = new CIntro_Kamen();

	if (FAILED(pInstance->Initilize(pDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIntro_Kamen");
		return nullptr;
	}

	return pInstance;
}

void CIntro_Kamen::Free()
{
	__super::Free();
}