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
	_float3* Get_PickingPos() { return m_pPickingPos; }

public:
	HRESULT Initialize_Manager();

#pragma region MAPDATA
	vector<MAP_DATA>* Get_MapData();
	HRESULT Load_MapData(const _char* pMapDataFilePaht);
	HRESULT Save_MapData(const _char* pMapDataFilePaht);
#pragma endregion

	

	void Bind_PickingPos(_float3* pPickingPos);
	

private:
	class CData_Manager*	m_pData_Manager = { nullptr };

	_float3*				m_pPickingPos = {};
public:
	virtual void Free() override;

	
};

NS_END