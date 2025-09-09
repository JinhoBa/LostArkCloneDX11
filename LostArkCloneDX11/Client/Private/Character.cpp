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

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}
