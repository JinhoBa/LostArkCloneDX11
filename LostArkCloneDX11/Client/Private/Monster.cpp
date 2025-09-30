#include "pch.h"
#include "Monster.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Body_Monster.h"
#include "HpBar_Monster.h"
#include "State.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy{ pDevice, pContext }
{
}

CMonster::CMonster(const CMonster& Prototype) 
	: CEnemy{ Prototype }
{
}

HIT_TYPE CMonster::Get_HitType()
{
	if (m_HitTypes.empty())
		return HIT_TYPE::END;

	HIT_TYPE eHitType = m_HitTypes.front();

	m_HitTypes.clear();

	return eHitType;
}

const _bool CMonster::isAnimationFinish()
{
	return dynamic_cast<CBody_Monster*>(Find_PartObject(TEXT("Body_Monster")))->isAnimationFinish();
}

void CMonster::Set_Animation(ANIMATIONSLOT eAnim)
{
	ANIMATION_DESC Desc = m_pGameManager->Get_AnimationIndex(m_iMonsetrID, eAnim);

	dynamic_cast<CBody_Monster*>(Find_PartObject(TEXT("Body_Monster")))->Set_Animation(Desc.iIndex, Desc.bLoop , Desc.fLerpTime);
}

void CMonster::Chase(_float fTimeDelta)
{
	_uint iNumMonster = {};

	list<CGameObject*> Monsters = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));

	_vector vOwnPositon = m_pTransformCom->Get_Position();
	_vector vSepration = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector vCohesion = vOwnPositon;
	
	for (auto& pMonster : Monsters)
	{
		if (this == pMonster)
			continue;

		if(5.f <= XMVectorGetX(XMVector3Length(pMonster->Get_Transform()->Get_Position() - m_pTransformCom->Get_Position())))
			continue;

		if (false == dynamic_cast<CMonster*>(pMonster)->isInBattle())
			continue;

		++iNumMonster;

		_vector vNeighborPositon = pMonster->Get_Transform()->Get_Position();

		/* Sepration */
		_vector vDir = vOwnPositon - vNeighborPositon;

		vDir = XMVectorSet(vDir.m128_f32[0], 0.f, vDir.m128_f32[2], 0.f);
		vSepration += vDir* (1.f / XMVectorGetX(XMVector3Length(vDir)));

		/* Cohesion */
		vCohesion += vNeighborPositon;
	}

	if(0 != iNumMonster)
	{
		vCohesion = (vCohesion / (_float)(iNumMonster + 1)) - vOwnPositon;
		vSepration = vSepration / (_float)iNumMonster;

		_vector vToTarget = (m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position()) * 0.3f +
			vSepration * 1.f + vCohesion * 0.3f;

		m_pTransformCom->Chase(fTimeDelta, XMVector3Normalize(vToTarget), vOwnPositon + vToTarget, m_fSpeed, m_pNavigationCom);
	}
	else
	{
		_vector vToTarget = (m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position());
		m_pTransformCom->Chase(fTimeDelta, XMVector3Normalize(vToTarget), vOwnPositon + vToTarget, m_fSpeed, m_pNavigationCom);
	}
}

_float CMonster::Get_TrackPositon()
{
	return static_cast<CBody_Monster*>(Find_PartObject(TEXT("Body_Monster")))->Get_TrackPoisiton();
}

void CMonster::Set_HitBox(_float3& vCenter, _float3& vExtends)
{
	m_pHitBoxCom->Set_ColliderDesc(vCenter, vExtends);
}
void CMonster::Update_HitBox(_uint iSkillID, _uint iHitIndex)
{
	isCollUpdate = true;
	m_iCurSkillID = iSkillID;
	m_iCurHitIndex = iHitIndex;

	m_pGameInstance->Add_Collider(TEXT("Monster_HitBox"), m_pHitBoxCom);
}

void CMonster::Set_Dead(_float fTime)
{
	m_isDead = true;
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{		
	m_fSpeed = 1.5f;

	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	m_iMonsetrID = pDesc->iMonsterID;
	m_iNumAttack = pDesc->iNumAttack;

	m_EnemyInfo.iMonsterID = m_iMonsetrID;
	m_EnemyInfo.fAttack = pDesc->fAttack;
	m_EnemyInfo.fAttackRange = pDesc->fAttackRange;
	m_EnemyInfo.fDetectDistance = pDesc->fDetectDistance;
	m_EnemyInfo.fHp = pDesc->fHp;
	m_EnemyInfo.fMaxHp = pDesc->fMaxHp;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pDesc->strModelPrototypeTag)))
		return E_FAIL;

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (nullptr == m_pPlayerTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pPlayerTransformCom);

	m_pTransformCom->Set_State(Engine::STATE::POSITION, XMLoadFloat4(&pDesc->vPosition));

	m_pRootBoneMatrix = dynamic_cast<CBody_Monster*>(Find_PartObject(TEXT("Body_Monster")))->Get_BoneMatrixPtr("b_root");

	if (nullptr == m_pRootBoneMatrix)
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	Detect_Player();

	if (isCollUpdate)
		m_pGameInstance->Check_Collider(m_pHitBoxCom, TEXT("Player"));

	__super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	//m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Ready_PartObjects(_wstring& strModelPrototypeTag)
{
	CBody_Monster::BODYMONSTER_DESC Body_Desc = {};
	Body_Desc.pParentTransform = m_pTransformCom;
	Body_Desc.strModelPrototypeTag = strModelPrototypeTag;
	if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster"), TEXT("Body_Monster"), &Body_Desc)))
		return E_FAIL;

	CHpBar_Monster::HPBAR_MONSTER_DESC  HpBar_Desc = {};
	HpBar_Desc.pMonsterInfo = &m_EnemyInfo;
	HpBar_Desc.pParentTransform = m_pTransformCom;
	HpBar_Desc.pSocketMatrix = dynamic_cast<CBody_Monster*>(Find_PartObject(TEXT("Body_Monster")))->Get_BoneMatrixPtr("b_effectname");
	if (FAILED(__super::Add_PartObject(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpBar_Monster"), TEXT("HPBar_Monster"), &HpBar_Desc)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Detect_Player()
{
	m_fDistToPlayer = XMVector4Length(m_pPlayerTransformCom->Get_Position() - m_pTransformCom->Get_Position()).m128_f32[0];

	if(false == m_bInBattle)
	{
		if (m_EnemyInfo.fDetectDistance >= m_fDistToPlayer)
			m_bInBattle = true;
	}
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pStateMachineCom);
	Safe_Release(m_pPlayerTransformCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pHitBoxCom);
}
