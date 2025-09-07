#include "pch.h"
#include "Monster.h"
#include "GameInstance.h"
#include "GameManager.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }, m_pGameManager{ CGameManager::GetInstance()}
{
	Safe_AddRef(m_pGameManager);
}

CMonster::CMonster(const CMonster& Prototype) 
	: CGameObject{ Prototype }, m_pGameManager{Prototype.m_pGameManager}
{
	Safe_AddRef(m_pGameManager);
}

void CMonster::Set_Animation(ANIMATIONSLOT eAnim)
{
	ANIMATION_DESC Desc = m_pGameManager->Get_AnimationIndex(m_iMonsetrID, eAnim);

	m_pModelCom->Set_AnimationIndex(m_pTransformCom, Desc.iIndex, Desc.bLoop , Desc.fLerpTime);
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

	m_isAnimationFinish = false;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pDesc->strModelPrototypeTag)))
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
}

void CMonster::Update(_float fTimeDelta)
{
	Detect_Player();
}

void CMonster::Late_Update(_float fTimeDelta)
{
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Ready_Components(_wstring& strPrototypeTag)
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), strPrototypeTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &m_pGameInstance->Get_LightDesc(0).vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &m_pGameInstance->Get_LightDesc(0).vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &m_pGameInstance->Get_LightDesc(0).vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &m_pGameInstance->Get_LightDesc(0).vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
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

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateMachineCom);

	Safe_Release(m_pGameManager);
}
