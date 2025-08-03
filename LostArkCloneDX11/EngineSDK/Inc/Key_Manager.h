#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	void Update_KeyInput();
	void End_KeyInput();

public:
	_bool KeyDown(_uint iKey);
	_bool KeyPressing(_uint iKey);
	_bool KeyUp(_uint iKey);

private:
	_bool m_PreKeyInput[MAX_KEY] = {};
	_bool m_CurKeyInput[MAX_KEY] = {};

public:
	static CKey_Manager* Create();
	virtual void Free() override;
};

NS_END