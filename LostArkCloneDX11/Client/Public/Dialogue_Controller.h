#pragma once
#include "Client_Defines.h"

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CDialogue_Controller : public CBase
{
private:
	CDialogue_Controller();
	virtual ~CDialogue_Controller() = default;

public:
	HRESULT		Initilize();
	void		Start_Dialogue(_uint iNpcID, _fvector vNpcPosition);
	void		End_Dialogue();

private:
	CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CDialogue_Controller* Create();
	virtual void Free() override;
};

NS_END
