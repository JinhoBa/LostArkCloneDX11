#pragma once
#include "Client_Defines.h"

#include "Base.h"
#include "GameManager.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CHit_Manager : public CBase
{
public:
	typedef struct tagHitDesc
	{
		CHARACTER eType;
		class CCharacter* pHitObject;
		_uint iSkillID;
		_uint iHitIndex;
		_float fBaseDamage;
	}HIT_DESC;
private:
	CHit_Manager();
	virtual ~CHit_Manager() = default;

public:
	void Add_HitDesc(void* pDesc);
	void Update_HitData();

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	class CGameManager*		m_pGameManager = { nullptr };

	list<HIT_DESC>			m_HitDesc_List;

public:
	static CHit_Manager* Create();
	virtual void Free() override;
};

NS_END
