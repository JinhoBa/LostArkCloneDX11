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

	m_iSoundIndex = 0;

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

	for (const auto& Track : EffectEvents)
	{
		m_EffectEvents.push_back(Track);
	}

	const vector<SOUND_EVENT_DESC>& SoundEvents = m_pGameManager->Get_SoundTrack(CHARACTER::BOSS, m_iEffectID);
	for (const auto& Track : SoundEvents)
	{
		m_SoundEvents.push_back(Track);
	}
}

void CState_Kamen::Update_EffectTrack()
{
	_float fTrackPosition = m_pKamen->Get_TrackPositon();

	auto iter = m_EffectEvents.begin();
	for ( ;iter != m_EffectEvents.end(); )
	{
		if (fTrackPosition >= (*iter).fKeyFrame)
		{
			m_pGameManager->Add_Effect((*iter).eType, (*iter).iID, m_pKamenWorldMatrix, CHARACTER::BOSS);

			iter = m_EffectEvents.erase(iter);
		}
		else
			++iter;
	}

	/* Sound */
	auto iter_Sound = m_SoundEvents.begin();

	for (; iter_Sound != m_SoundEvents.end();)
	{
		if (fTrackPosition >= (*iter_Sound).fKeyFrame)
		{
			CHANNELID eChannel = {};

			if (0 == m_iSoundIndex)
				eChannel = CHANNELID::SKILL_BOSS0;
			else if(1 == m_iSoundIndex)
				eChannel = CHANNELID::SKILL_BOSS1;
			else
			{
				eChannel = CHANNELID::SKILL_BOSS2;
			}

			m_pGameInstance->Play_Sound((*iter_Sound).strFileName.data(), eChannel, (*iter_Sound).fVolume);
			iter_Sound = m_SoundEvents.erase(iter_Sound);

			++m_iSoundIndex;
			if (2 < m_iSoundIndex)
				m_iSoundIndex = 0;
		}
		else
			++iter_Sound;
	}
}

void CState_Kamen::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}