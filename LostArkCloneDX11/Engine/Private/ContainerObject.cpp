#include "ContainerObject.h"

#include "GameInstance.h"

#include "PartObject.h"

CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CContainerObject::CContainerObject(const CContainerObject& Prototype)
	:CGameObject{Prototype}
{
}

HRESULT CContainerObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainerObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	return S_OK;
}

void CContainerObject::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject.second->Priority_Update(fTimeDelta);
	}
}

void CContainerObject::Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject.second->Update(fTimeDelta);
	}
}

void CContainerObject::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject.second->Late_Update(fTimeDelta);
	}
}

HRESULT CContainerObject::Render()
{
	return S_OK;
}

CPartObject* CContainerObject::Find_PartObject(const _wstring& PartObjectTag)
{
	auto iter = m_PartObjects.find(PartObjectTag);

	if (m_PartObjects.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT CContainerObject::Add_PartObject(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& PartObjectTag, void* pArg)
{
	if (nullptr != Find_PartObject(PartObjectTag))
		return E_FAIL;

	CPartObject* pObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLevelIndex, strPrototypeTag, pArg));

	if (nullptr == pObject)
		return E_FAIL;

	m_PartObjects.emplace(PartObjectTag, pObject);

	return S_OK;
}

void CContainerObject::Free()
{
	__super::Free();

	for (auto& PartObject : m_PartObjects)
	{
		Safe_Release(PartObject.second);
	}
	m_PartObjects.clear();
}
