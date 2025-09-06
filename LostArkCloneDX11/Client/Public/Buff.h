#pragma once
#include"Client_Defines.h"
#include"Client_Struct.h"
#include "Base.h"

NS_BEGIN(Client)

class CBuff final : public CBase
{
private:
	CBuff();
	virtual ~CBuff() = default;

public:
	const _bool isFinished() const { return m_isFinished; }
	const _uint Get_BuffID() const { return m_iBuffID; }
public:
	HRESULT Initilize(_uint iBuffID);
	void	Update(_float fTimeDelta, BUFFSTAT* pStat );
	void	Reset();

private:
	_bool		m_isFinished = {};
	_uint		m_iBuffID = {};

	_float		m_fTimeAcc = {};
	_float		m_fDuration = {};

	BUFFSTAT	m_BuffStat = {};

public:
	static CBuff* Create();
	virtual void Free() override;
};

NS_END