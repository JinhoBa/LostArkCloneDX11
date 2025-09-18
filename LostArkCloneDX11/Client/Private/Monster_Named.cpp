#include "pch.h"
#include "Monster_Named.h"

#include "GameInstance.h"

#include "Monster_Idle.h"
#include "Monster_Attack.h"
#include "Monster_Turn.h"
#include "Monster_Run.h"
#include "Monster_Dead.h"
#include "Monster_Hit.h"

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

	m_pGameInstance->Add_Collider(TEXT("Monster"), m_pColliderCom);
}

void CMonster_Named::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pStateMachineCom->Upadte(fTimeDelta);

	Check_Navigation(m_pNavigationCom, m_pRootBoneMatrix);

	m_pColliderCom->Update(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

	if (!m_HitTypes.empty())
		m_pStateMachineCom->Change_State(Get_State(CMonster::STATE::HIT), nullptr);
}

void CMonster_Named::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	m_pColliderCom->Update_OnCollision();
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster_Named::Render()
{
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

	m_pColliderCom->Set_OnCollisionEnter([&]() { 
		//m_pTransformCom->TurnTo(m_pColliderCom->Get_HitBoxDesc().pHitObject->Get_Transform()->Get_Position());
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
	__super::Free();

	Safe_Release(m_pColliderCom);
}