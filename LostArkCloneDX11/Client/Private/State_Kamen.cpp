#include "pch.h"
#include "State_Kamen.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Kamen.h"
#include "StateMachine.h"

CState_Kamen::CState_Kamen()
	:CState{}, m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CState_Kamen::Initilize(STATE_KAMEN_DESC* pDesc)
{
	if (nullptr == pDesc)
		return E_FAIL;

	m_pStateMachine = pDesc->pStateMachine;
	m_pKamen = pDesc->pKamen;
	m_pPhase = pDesc->pPhase;
	
	m_pKamenWorldMatrix = &m_pKamen->Get_Transform()->Get_WorldMatrix();

	return S_OK;
}

void CState_Kamen::Enter(void* pArg)
{
}

void CState_Kamen::Update(_float fTimeDelta)
{
}

void CState_Kamen::Exit()
{
}

void CState_Kamen::Ready_EffectEvents()
{
	/* Effects */
	const vector<EFFECT_EVENT_DESC>& EffectEvents = m_pGameManager->Get_EffectTrack(CHARACTER::BOSS, m_iEffectID);

	m_EffectEvents.reserve(EffectEvents.size());

	for (const auto& Track : EffectEvents)
	{
		m_EffectEvents.push_back({ false, Track });
	}
}

void CState_Kamen::Update_EffectTrack()
{
	for (auto& Event : m_EffectEvents)
	{
		if (false == Event.isTrigge)
		{
			if (m_pKamen->Get_TrackPositon() >= Event.EventDesc.fKeyFrame)
			{
				Event.isTrigge = true;
				m_pGameManager->Add_Effect(Event.EventDesc.eType, Event.EventDesc.iID, m_pKamenWorldMatrix, CHARACTER::BOSS);
			}
		}
	}
}

void CState_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}