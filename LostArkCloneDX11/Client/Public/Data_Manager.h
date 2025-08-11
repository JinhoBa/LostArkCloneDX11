#pragma once
#include "Client_Defines.h"

#include "Base.h"
NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CData_Manager : public CBase
{
private:
	CData_Manager();
	virtual ~CData_Manager() = default;

public:
	HRESULT Load_File(const _char* pFilePath);
	HRESULT Save_File(const _char* pFilePath);

	vector<MAP_DATA>* Get_Data() { return &m_MapDatas; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	vector<MAP_DATA> m_MapDatas;
	
public:
	static CData_Manager* Creat();
	virtual void Free() override;
};

NS_END