#pragma once
#include "Client_Defines.h"

#include "Base.h"
#include "GameManager.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CDamageFont_Manager : public CBase
{
private:
	CDamageFont_Manager();
	virtual ~CDamageFont_Manager() = default;

public:
	HRESULT Initilize();

public:
	void Add_DamageFont(DAMAGEFONT eType, _float fDamage, _float3& vPostion);
	void Update_DamageFont(_float fTimeDelta);

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	class CGameManager*		m_pGameManager = { nullptr };

	list<pair<FONT_DESC, _float4>> m_DamageFonts;

public:
	static CDamageFont_Manager* Create();
	virtual void Free() override;
};

NS_END
