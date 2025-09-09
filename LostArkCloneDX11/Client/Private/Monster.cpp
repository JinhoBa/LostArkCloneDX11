#include "pch.h"
#include "Monster.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Body_Monster.h"
#include "HpBar_Monster.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter{ pDevice, pContext }
{
	Safe_AddRef(m_pGameManager);
}

CMonster::CMonster(const CMonster& Prototype) 
	: CCharacter{ Prototype }
{
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
	m_pTransformCom->MoveTo(fTimeDelta, m_pPlayerTransformCom->Get_Position(), 1.5f);
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{		
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	m_iMonsetrID = pDesc->iMonsterID;
	m_iNumAttack = pDesc->iNumAttack;

	m_MonsterInfo.fAttack = pDesc->fAttack;
	m_MonsterInfo.fAttackRange = pDesc->fAttackRange;
	m_MonsterInfo.fDetectDistance = pDesc->fDetectDistance;
	m_MonsterInfo.fHp = pDesc->fHp;
	m_MonsterInfo.fMaxHp = pDesc->fMaxHp;
	
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

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	Detect_Player();

	__super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
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
	HpBar_Desc.pMonsterInfo = &m_MonsterInfo;
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
		if (m_MonsterInfo.fDetectDistance >= m_fDistToPlayer)
			m_bInBattle = true;
	}
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransformCom);
	Safe_Release(m_pStateMachineCom);

}
