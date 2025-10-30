#include "pch.h"
#include "Player_AwakeSkill.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "StateMachine.h"
#include "Player.h"
#include "Effect_Ground.h"
#include "WeaponEffect_Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"
#include "Camera_Fix.h"

CPlayer_AwakeSkill::CPlayer_AwakeSkill()
	:CSkill_Player{}
{
}

HRESULT CPlayer_AwakeSkill::Initilize(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	if (FAILED(__super::Initilize(pStateMachine, pStance, pPlayer)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_AwakeSkill::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pPlayer->TurnToCursor();

	AWAKE_SKILL_DESC* m_pSkill_Desc = static_cast<AWAKE_SKILL_DESC*>(pArg);

	m_iSkillID = m_pSkill_Desc->iSkillID;
	m_pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(m_pSkill_Desc->iSkillID);
	m_eHitboxType = COLLIDER::SPHERE;

	m_pPlayer->Set_Animation(m_pSkill_Desc->iAnimationIndex, m_pSkill_Desc->bLoop);



	/* Effects */
	Ready_EffectTrack();


	m_pPlayer->Toggle_PartObject(L"Weapon_Player");

	/* ÀÌÆåÆ® À§Ä¡ */
	XMStoreFloat4(&m_vPickingPosition, m_pGameManager->Picking_Terrains());

	_vector vDir = XMLoadFloat4(&m_vPickingPosition) - m_pPlayer->Get_Transform()->Get_Position();

	if (5.f < XMVectorGetX(XMVector3Length(vDir)))
	{
		XMStoreFloat4(&m_vPickingPosition, m_pPlayer->Get_Transform()->Get_Position() + XMVector3Normalize(vDir) * 5.f);
	}

	_float3 vHitBoxPostition = _float3(m_vPickingPosition.x, m_vPickingPosition.y, m_vPickingPosition.z);
	
	// XMStoreFloat3(&vHitBoxOffset, XMVectorGetX(XMVector3Length(vDir))* XMVectorSet(1.f, 0.f, 0.f, 0.f));


	/* HitBox */
	m_pPlayer->Set_HitBox(m_pSkillInfo->HitBoxDesc.vOffset, m_pSkillInfo->HitBoxDesc.vExtends, vHitBoxPostition, COLLIDER::SPHERE);

	if (m_pSkillInfo->bApplyRimLightBody)
		dynamic_cast<CBody_Player*>(m_pPlayer->Get_PartObject(L"Body_Player"))->Toggle_RimLight();
	if (m_pSkillInfo->bApplyRimLightWeapon)
		dynamic_cast<CWeapon_Player*>(m_pPlayer->Get_PartObject(L"Weapon_Player"))->Toggle_RimLight();

}

void CPlayer_AwakeSkill::Update(_float fTimeDelta)
{
	m_pPlayer->Check_Navi();

	Update_Hitbox(fTimeDelta);

	_float fTrackPosition = m_pPlayer->Get_TrackPositon();

	auto iter_Effect = m_EffectEvents.begin();

	for (; iter_Effect != m_EffectEvents.end();)
	{
		if (fTrackPosition >= (*iter_Effect).fKeyFrame)
		{
			_float4x4 MouseWorldMartix;
			XMStoreFloat4x4(&MouseWorldMartix, XMLoadFloat4x4(m_pPlayerWorldMatrix));

			MouseWorldMartix._41 = m_vPickingPosition.x;
			MouseWorldMartix._43 = m_vPickingPosition.z;

			m_pGameManager->Add_Effect((*iter_Effect).eType, (*iter_Effect).iID, &MouseWorldMartix, CHARACTER::PLAYER);

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

	/* Sound */
	auto iter_Sound = m_SoundEvents.begin();

	for (; iter_Sound != m_SoundEvents.end();)
	{
		if (fTrackPosition >= (*iter_Sound).fKeyFrame)
		{
			m_pGameInstance->Play_Sound((*iter_Sound).strFileName.data(), CHANNELID::SKILL_PLAYER, (*iter_Sound).fVolume);
			iter_Sound = m_SoundEvents.erase(iter_Sound);
		}
		else
			++iter_Sound;
	}

	if (false == m_isSpawWeaponEffect && 34.f <= fTrackPosition && 50.f >= fTrackPosition)
	{
		m_isSpawWeaponEffect = true;
		m_pPlayer->Toggle_PartObject(L"WeaponEffect_Player");
	}
	else if (true == m_isSpawWeaponEffect && 115.f <= fTrackPosition)
	{
		m_isSpawWeaponEffect = false;
		dynamic_cast<CWeaponEffect_Player*>(m_pPlayer->Get_PartObject(L"WeaponEffect_Player"))->Shoot(&m_vPickingPosition);
	}


	if (m_pPlayer->isAnimationFinish())
	{
		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::RBUTTON))
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::MOVE), nullptr);
		else
			m_pStateMachine->Change_State(m_pPlayer->Get_State(CPlayer::STATE::IDLE), nullptr);
	}
}

void CPlayer_AwakeSkill::Exit()
{
	m_pPlayer->Toggle_PartObject(L"Weapon_Player");

	Clear_Events();

	m_pPlayer->Set_SkillID(99);
}

void CPlayer_AwakeSkill::Play_SkillSound()
{
	//m_pGameInstance->Play_Sound(L"Player_Ultimate1_Ready.wav", CHANNELID::SKILL_PLAYER, 0.8f);
}

CPlayer_AwakeSkill* CPlayer_AwakeSkill::Create(CStateMachine* pStateMachine, STANCE* pStance, CPlayer* pPlayer)
{
	CPlayer_AwakeSkill* pInstance = new CPlayer_AwakeSkill();

	if (FAILED(pInstance->Initilize(pStateMachine, pStance, pPlayer)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPlayer_AwakeSkill");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_AwakeSkill::Free()
{
	__super::Free();
}