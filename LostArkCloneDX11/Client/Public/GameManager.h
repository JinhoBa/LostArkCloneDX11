#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager);
private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	HRESULT Initialize_Manager();
	HRESULT Load_MapData(const _char* pMapDataFilePaht);
	HRESULT Save_MapData(const _char* pMapDataFilePaht);

private:
	class CData_Manager* m_pData_Manager = { nullptr };

public:
	virtual void Free() override;

	
};

NS_END