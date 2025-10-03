#pragma once
#include "Client_Defines.h"

#include "Base.h"
#include "GameManager.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CEffect_Manager : public CBase
{
private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Initilize();

public:
	void Add_Effects(const _tchar* pEffectTag, class CEffect* pEffect);


private:
	CGameInstance* m_pGameInstance = { nullptr };

	map<_wstring, class CEffect*> m_Effects;

public:
	static CEffect_Manager* Create();
	virtual void Free() override;
};

NS_END
