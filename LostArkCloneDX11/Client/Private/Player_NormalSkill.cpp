#include "pch.h"
#include "Player_NormalSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Effect_Ground.h"
#include "WeaponEffect_Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"

CPlayer_NormalSkill::CPlayer_NormalSkill()
	:CSkill_Player{}
{
}

HRESULT CPlayer_NormalSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_NormalSkill::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pPlayer->TurnToCursor();

	NORMALSKILL_DESC* m_pSkill_Desc = static_cast<NORMALSKILL_DESC*>(pArg);

	m_iSkillID = m_pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_pSkill_Desc->iSkillID);

	if(10 == m_iSkillID)
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop, 0.f);
	else
		m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop);

	/* Sound */
	Play_SkillSound();

	/* HitBox */
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends);

	/* Effects */
	const vector<EFFECT_EVENT_DESC>& EffectEvents = m_pGameManager->Get_EffectTrack(CHARACTER::PLAYER, m_iSkillID);

	m_EffectEvents.reserve(EffectEvents.size());

	for (const auto& Track : EffectEvents)
	{
		m_EffectEvents.push_back({ false, Track });
	}

	if (14 == m_iSkillID)
	{
		m_pPlayer->Toggle_PartObject(L"Weapon_Player");
		
		/* ÀÌÆåÆ® À§Ä¡ */
		XMStoreFloat4(&m_vPickingPosition, m_pGameManager->Picking_Terrains());

		_vector vDir = XMLoadFloat4(&m_vPickingPosition) - m_pPlayer->Get_Transform()->Get_Position();

		if (5.f < XMVectorGetX(XMVector3Length(vDir)))
		{
			XMStoreFloat4(&m_vPickingPosition, m_pPlayer->Get_Transform()->Get_Position() + XMVector3Normalize(vDir) * 5.f);
		}
	}

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();
}

void CPlayer_NormalSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	Update_Hitbox(fTimeDelta);
	if (14 == m_iSkillID)
	{
		for (auto& Event : m_EffectEvents)
		{
			if (false == Event.isTrigge)
			{
				if (m_pPlayer->Get_TrackPositon() >= Event.EventDesc.fKeyFrame)
				{
					_float4x4 MouseWorldMartix;
					XMStoreFloat4x4(&MouseWorldMartix, XMLoadFloat4x4(m_pPlayerWorldMatrix));
					
					MouseWorldMartix._41 = m_vPickingPosition.x;
					MouseWorldMartix._43 = m_vPickingPosition.z;

					Event.isTrigge = true;
					m_pGameManager->Add_Effect(Event.EventDesc.eType, Event.EventDesc.iID, &MouseWorldMartix, CHARACTER::PLAYER);
				}
			}
		}

		if (false == m_isSpawWeaponEffect && 34.f <= m_pPlayer->Get_TrackPositon() && 50.f >= m_pPlayer->Get_TrackPositon())
		{
			m_isSpawWeaponEffect = true;
			m_pPlayer->Toggle_PartObject(L"WeaponEffect_Player");
		}
		else if (true == m_isSpawWeaponEffect && 115.f <= m_pPlayer->Get_TrackPositon())
		{
			m_isSpawWeaponEffect = false;
			dynamic_cast<CWeaponEffect_Player*>(m_pPlayer->Get_PartObject(L"WeaponEffect_Player"))->Shoot(&m_vPickingPosition);
		}
	}
	else
		Update_EffectTrack();

	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_NormalSkill::Exit()
{
	if(14 == m_iSkillID)
		m_pPlayer->Toggle_PartObject(L"Weapon_Player");

	//m_pGameInstance->StopSound(CHANNELID::SKILL_PLAYER);

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

	m_pPlayer->Set_SkillID(99);

	m_EffectEvents.clear();
}

void CPlayer_NormalSkill::Play_SkillSound()
{
	switch (m_iSkillID)
	{
	case 1:
		m_pGameInstance->Play_Sound(L"Moonblade_Fury.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 3:
		m_pGameInstance->Play_Sound(L"RingofLight.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 4:
		m_pGameInstance->Play_Sound(L"DescentOfMoon.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 5:
		m_pGameInstance->Play_Sound(L"EclipsePath.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 6:
		m_pGameInstance->Play_Sound(L"SkyWheel.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 7:
		m_pGameInstance->Play_Sound(L"CrescentMoon.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 10:
		m_pGameInstance->Play_Sound(L"SolarEclispe.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 11:
		m_pGameInstance->Play_Sound(L"LightBlades.wav", CHANNELID::SKILL_PLAYER, 0.2f);
		break;
	case 14:
		m_pGameInstance->Play_Sound(L"Ultimate1.wav", CHANNELID::SKILL_PLAYER, 0.4f);
		break;
	default:
		break;
	}
}

CPlayer_NormalSkill* CPlayer_NormalSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_NormalSkill* pInstance = new CPlayer_NormalSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_NormalSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_NormalSkill::Free()
{
	__super::Free();
}