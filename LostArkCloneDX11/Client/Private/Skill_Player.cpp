#include "pch.h"
#include "Skill_Player.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"
#include "Camera_Fix.h"

CSkill_Player::CSkill_Player()
	:CState_Player{}
{
}

HRESULT CSkill_Player::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	m_pPlayerWorldMatrix = &pPlayer->Get_Transform()->Get_WorldMatrix();

	return S_OK;
}

void CSkill_Player::Enter(void* pArg)
{
	m_isStartHit = m_isActiveHitBox = false;
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;
}

void CSkill_Player::Update(_float fTimeDelta)
{
}

void CSkill_Player::Exit()
{
}

void CSkill_Player::Update_Hitbox(_float fTimeDelta)
{
	if (m_isStartHit == false && m_pPlayer->Get_TrackPositon() >= m_pSkillInfo->HitBoxDesc.fStartTime)
	{
		m_isStartHit = m_isActiveHitBox = true;
	}

	if (true == m_isStartHit && m_iAttackCount < m_pSkillInfo->iNumAttack)
	{
		if (m_isActiveHitBox)
		{
			m_fTimeAcc += fTimeDelta;
			m_pPlayer->Update_HitBox(m_iSkillID, m_iAttackCount);

			if (m_pSkillInfo->HitBoxDesc.fDuration <= m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				m_isActiveHitBox = false;
			}
		}
		else
		{
			m_fTimeAcc += fTimeDelta;
			if (m_pSkillInfo->HitBoxDesc.fInterval <= m_fTimeAcc)
			{
				m_isActiveHitBox = true;
				m_fTimeAcc = 0.f;
				++m_iAttackCount;
			}
		}
	}
}

void CSkill_Player::Update_EffectTrack()
{
	_float fTrackPosition = m_pPlayer->Get_TrackPositon();

	auto iter_Effect = m_EffectEvents.begin();

	for (; iter_Effect != m_EffectEvents.end();)
	{
		if (fTrackPosition >= (*iter_Effect).fKeyFrame)
		{
			m_pGameManager->Add_Effect((*iter_Effect).eType, (*iter_Effect).iID, m_pPlayerWorldMatrix, CHARACTER::PLAYER);
			iter_Effect = m_EffectEvents.erase(iter_Effect);
		}
		else
			++iter_Effect;
	}

	auto iter_CameraShake = m_CameraShakeEvents.begin();

	for (;iter_CameraShake != m_CameraShakeEvents.end();)
	{
		if (fTrackPosition >= (*iter_CameraShake).fKeyFrame)
		{
			dynamic_cast<CCamera_Fix*>(m_pGameInstance->Find_Camera(TEXT("Camera_Fix")))->Start_Shake((*iter_CameraShake).fTime);
			iter_CameraShake = m_CameraShakeEvents.erase(iter_CameraShake);
		}
		else
			++iter_CameraShake;
	}

	auto iter_Blur = m_BlurEvents.begin();

	for (;iter_Blur != m_BlurEvents.end();)
	{
		if (fTrackPosition >= (*iter_Blur).fKeyFrame)
		{
			m_pGameManager->Start_ScreenBlur((*iter_Blur).fTime);
			iter_Blur = m_BlurEvents.erase(iter_Blur);
		}
		else
			++iter_Blur;
	}
}

void CSkill_Player::Ready_EffectTrack()
{
	const vector<EFFECT_EVENT_DESC>& EffectEvents = m_pGameManager->Get_EffectTrack(CHARACTER::PLAYER, m_iSkillID);

	for (const auto& Track : EffectEvents)
	{
		m_EffectEvents.push_back(Track);
	}

	const vector<CAMERA_SHAKE_EVENT_DESC>& CameraEvents = m_pGameManager->Get_Camera_Track(m_iSkillID);

	for (const auto& Track : CameraEvents)
	{
		m_CameraShakeEvents.push_back(Track);
	}

	const vector<BLUR_EVENT_DESC>& BlurEvents = m_pGameManager->Get_BlurTrack(m_iSkillID);

	for (const auto& Track : BlurEvents)
	{
		m_BlurEvents.push_back(Track);
	}

}

void CSkill_Player::Clear_Events()
{
	m_BlurEvents.clear();
	m_CameraShakeEvents.clear();
	m_EffectEvents.clear();
}

void CSkill_Player::Free()
{
	__super::Free();
}