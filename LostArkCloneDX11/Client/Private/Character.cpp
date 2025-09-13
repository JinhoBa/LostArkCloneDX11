#include "pch.h"
#include "Character.h"

#include "GameManager.h"
#include "GameInstance.h"

CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice, pContext }, 
	m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

CCharacter::CCharacter(const CCharacter& Prototype)
	:CContainerObject{ Prototype },
	m_pGameManager{ Prototype.m_pGameManager }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	m_PreRootBonePosition = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CCharacter::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CCharacter::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCharacter::Render()
{
	return S_OK;
}

void CCharacter::Check_Navigation(CNavigation* pNavigation,const _float4x4* pRootBoneMatrix)
{
	_vector vRootBonePosition = XMVector3TransformCoord(
	   XMVectorSet(pRootBoneMatrix->_41, pRootBoneMatrix->_42, pRootBoneMatrix->_43, 1.f),
	   XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

   _vector vDist = XMLoadFloat4(&m_PreRootBonePosition) - vRootBonePosition;

   if (false == pNavigation->isMove(vRootBonePosition))
	   m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pTransformCom->Get_Position() + XMVectorSetY(vDist,0.f));
   else
	   XMStoreFloat4(&m_PreRootBonePosition, vRootBonePosition);
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}
