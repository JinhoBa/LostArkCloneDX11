#include "pch.h"
#include "Monster_Named.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster_Idle.h"
#include "Monster_Attack.h"
#include "Monster_Turn.h"
#include "Monster_Run.h"
#include "Monster_Dead.h"
#include "Monster_Hit.h"

#include "Player.h"
#include "State.h"

CMonster_Named::CMonster_Named(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CMonster_Named::CMonster_Named(const CMonster_Named& Prototype)
	:CMonster{Prototype}
{
}

HRESULT CMonster_Named::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Named::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_eType = MONSTER::NAMED;

	Set_Animation(ANIMATIONSLOT::IDLE);

	m_pStateMachineCom->Start_State(m_States[IDLE]);

	return S_OK; 
}

void CMonster_Named::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if(0.f <= m_EnemyInfo.fHp)
		m_pGameInstance->Add_Collider(TEXT("Monster"), m_pColliderCom);

#ifdef _DEBUG
	isCollUpdate = false;
#endif // _DEBUG

}

void CMonster_Named::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pStateMachineCom->Upadte(fTimeDelta);

	Check_Navigation(m_pNavigationCom, m_pRootBoneMatrix);

	m_pColliderCom->Update(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
	m_pHitBoxCom->Update(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

	if (!m_HitTypes.empty() && 0.f <=m_EnemyInfo.fHp)
		m_pStateMachineCom->Change_State(Get_State(CMonster::STATE::HIT), nullptr);

	if (isCollUpdate)
		m_pGameInstance->Check_Collider(m_pHitBoxCom, TEXT("Player"));
}

void CMonster_Named::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	m_pColliderCom->Update_OnCollision();
	m_pHitBoxCom->Update_OnCollision();



	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster_Named::Render()
{
#ifdef _DEBUG
	
	/* TEST *//*
	ImGui::SliderFloat3("HitPos1", reinterpret_cast<_float*>(&m_vHitBoxCenter), -3.f, 3.f);
	ImGui::SliderFloat3("HitExtents1", reinterpret_cast<_float*>(&m_vHitBoxExtents), 0.3f, 3.f);*/

	if(isCollUpdate)
		m_pHitBoxCom->Render();
	m_pNavigationCom->Render();

#endif // _DEBUG
	m_pColliderCom->Render();

	return S_OK;
}
HRESULT CMonster_Named::Ready_Components()
{
	/* StateMachine */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC Desc = {};

	Desc.iCurrentIndex = 0;

	/* StateMachine */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Trision"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
		return E_FAIL;

	/* Collider */
	CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.vCenter = _float3(0.f, 0.5f, 0.f);
	AABB_Desc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	AABB_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABB_Desc)))
		return E_FAIL;

	/* Collider */
	CBounding_OBB::BOUNDING_OBB_DESC HitBox_Desc = {};
	HitBox_Desc.vCenter = _float3(0.f, 0.5f, 0.f);
	HitBox_Desc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	HitBox_Desc.vOrientation = _float3(0.f, 0.f, 0.f);
	HitBox_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_HitBox_OBB"), reinterpret_cast<CComponent**>(&m_pHitBoxCom), &HitBox_Desc)))
		return E_FAIL;

	m_pHitBoxCom->Set_OnCollisionEnter([&]() {
		deque<CGameObject*>& Objects = m_pHitBoxCom->Get_HitObjects();

		MONSTER_SKILL_INFO* pSkill = m_pGameManager->Get_Monster_SkillInfo_Prt(m_iMonsetrID, m_iCurSkillID);

		ATTACK_DESC Desc = {};
		Desc.eHitType = pSkill->eHitType;
		Desc.eAttackType = ATTACK_TYPE::NORMAL;
		_float fDamage = Desc.fDamage = pSkill->HitBoxDescs[m_iCurHitIndex].fDamage;

		while (!Objects.empty())
		{
			_float3 vPosition;
			XMStoreFloat3(&vPosition, Objects.front()->Get_Transform()->Get_Position());

			m_pGameManager->Add_DamageFont(DAMAGEFONT::PLAYER_HURT, fDamage, vPosition);
			dynamic_cast<CPlayer*>(Objects.front())->OnHit(Desc);
			Objects.pop_front();
		}
		});


	return S_OK;
}

HRESULT CMonster_Named::Ready_States()
{
	m_States[IDLE] = CMonster_Idle::Create(m_pStateMachineCom, &m_EnemyInfo, this);
	m_States[ATTACK] = CMonster_Attack::Create(m_pStateMachineCom, &m_EnemyInfo, this, m_iNumAttack);
	m_States[TURN] = CMonster_Turn::Create(m_pStateMachineCom, &m_EnemyInfo, this);
	m_States[RUN] = CMonster_Run::Create(m_pStateMachineCom, &m_EnemyInfo, this);
	m_States[DEAD] = CMonster_Dead::Create(m_pStateMachineCom, &m_EnemyInfo, this);
	m_States[HIT] = CMonster_Hit::Create(m_pStateMachineCom, &m_EnemyInfo, this);

	return S_OK;
}

CMonster_Named* CMonster_Named::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Named* pInstance = new CMonster_Named(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Stancd ");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster_Named::Clone(void* pArg)
{
	CGameObject* pInstance = new CMonster_Named(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CMonster_Stancd ");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Named::Free()
{
	for (_uint i = 0; i < STATE::STATE_END; i++)
	{
		Safe_Release(m_States[i]);
	}

	__super::Free();
}