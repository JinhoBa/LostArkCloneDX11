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
private:
	CClash_Manager();
	virtual ~CClash_Manager() = default;

public:
	HRESULT Initilize();

public:
	void Update_Clash(_float fTimeDelta);

private:
	CGameInstance* m_pGameInstance = { nullptr };
	class CGameManager* m_pGameManager = { nullptr };

	_uint m_iNumSuccess = {};


public:
	static CClash_Manager* Create();
	virtual void Free() override;
};

NS_END
