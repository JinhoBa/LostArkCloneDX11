#include "Collider_Manager.h"

#include "Collider.h"

CCollider_Manager::CCollider_Manager()
{

}

HRESULT CCollider_Manager::Initialize()
{

	return S_OK;
}

_bool CCollider_Manager::Check_Collider(CCollider* pColldier, const _tchar* pSrcLayerTag)
{
	auto iter = m_ColliderLists.find(pSrcLayerTag);

	if (m_ColliderLists.end() == iter)
		return false;

	_bool isColl = false;

	for (auto& pSrcColldier : (*iter).second)
	{
		if (true == pColldier->Intersect(pSrcColldier))
		{
			isColl = true;
			
			pSrcColldier->Hurt(pColldier);
		}
	}
	pColldier->Hit(isColl);

	return isColl;
}

void CCollider_Manager::Update_Collider()
{
	for (auto pair : m_ColliderLists)
	{
		for (auto pCollider : pair.second)
			pCollider->Update_OnCollision();
	}
}

void CCollider_Manager::Clear_Collider()
{
	for (auto& ColliderList : m_ColliderLists)
	{
		ColliderList.second.clear();
	}
}

void CCollider_Manager::Add_Collider(const _tchar* pLayerTag, class CCollider* pCollider)
{
	auto iter = m_ColliderLists.find(pLayerTag);

	if (m_ColliderLists.end() == iter)
	{
		list<CCollider*> ColliderList;
		ColliderList.push_back(pCollider);
		m_ColliderLists.emplace(pLayerTag, ColliderList);
	}
	else
	{
		(*iter).second.push_back(pCollider);
	}
}

CCollider_Manager* CCollider_Manager::Create()
{
	CCollider_Manager* pInstance = new CCollider_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CCollider_Manager");
		return nullptr;
	}

	return pInstance;
}

void CCollider_Manager::Free()
{
	__super::Free();
}