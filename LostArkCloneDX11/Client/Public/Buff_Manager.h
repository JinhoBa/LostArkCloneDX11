#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CBuff_Manager final : public CBase
{
private:
	CBuff_Manager();
	virtual ~CBuff_Manager() = default;

public:
	HRESULT Initilize();

	class CBuff* Add_Buff(_uint iBuffID);
	void Remove_Buff(class CBuff* pBuff);

private:
	list<class CBuff*> m_Buffs= {};

public:
	static CBuff_Manager* Create();
	virtual void Free() override;
};

NS_END