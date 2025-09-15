#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCollider_Manager final : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Initialize();
	_bool	Check_Collider(class CCollider* pColldier, const _tchar* pDestLayerTag);
	void	Update_Collider();
	void	Clear_Collider();
	void	Add_Collider(const _tchar* pLayerTag, class CCollider* pCollider);

private:
	map<_wstring, list<class CCollider*>>		m_ColliderLists;

public:
	static CCollider_Manager* Create();
	virtual void Free() override;
};

NS_END