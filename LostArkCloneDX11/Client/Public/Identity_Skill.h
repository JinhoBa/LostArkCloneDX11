#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Base.h"

NS_BEGIN(Client)

class CIdentity_Skill final : public CBase
{
private:
	CIdentity_Skill();
	virtual ~CIdentity_Skill() = default;

public:
	HRESULT Initialize(PLAYER_INFO* pPlayerInfo);
	void Update(_float fTimeDelta);

private:
	PLAYER_INFO*	m_pPlayerInfo = { nullptr };

	_float			m_fCoolTime = {};
	_float			m_fMaxfCoolTime = {};

public:
	static CIdentity_Skill* Create(PLAYER_INFO* pPlayerInfo);
	virtual void Free() override;
};

NS_END