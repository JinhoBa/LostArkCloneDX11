#pragma once
#include "Client_Defines.h"

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CClash_Manager : public CBase
{
	typedef struct tagClashData
	{
		_int		iKey;
		_float		fOffsetTime;
		_float2		vPosition;
	}CLASH_DATA;

private:
	CClash_Manager();
	virtual ~CClash_Manager() = default;

public:
	_bool	isFinshed_Clash() const { return m_isFinish; }

public:
	HRESULT		Initilize();
	_uint		Update_Clash(_float fTimeDelta);

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	class CGameManager*			m_pGameManager = { nullptr };

	_bool						m_isFinish = {};
	_uint						m_iNumSuccess = {};
	_uint						m_iPatternIndex = {};
	_uint						m_iIndex = {};

	_float						m_fPlayTime = {};
	_float						m_fCreateTime = {};

	vector<CLASH_DATA>			m_Datas[2];
	function<void()>			m_EndEvent;

private:
	void	Add_ClashUI();
	void	Ready_Data();

public:
	static CClash_Manager* Create();
	virtual void Free() override;
};

NS_END
