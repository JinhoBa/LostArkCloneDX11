#pragma once

#include "Base.h"

NS_BEGIN(Engine)
#ifdef _DEBUG
class ENGINE_DLL CNavigation_Tool final : public CBase
{
private:
	CNavigation_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNavigation_Tool() = default;

public:
	virtual HRESULT Initialize();

	void Update(_fmatrix WorldMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	}

	_bool Pick_Cell(_fvector vPickingPosition, _float3* pPoint);

	HRESULT	Add_Sell(const _float3* pPoints);
	void Remove_Sell();
	HRESULT Save_File(const _char* pNavigationFilePath);
	HRESULT LoadFile(const _char* pNavigationFilePath);


public:
	HRESULT Render();


private:
	class CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

	_uint							m_iNumCells = {};
	_int							m_iCurrentCellIndex = { -1 };
	vector<class CCell*>			m_Cells;
	_float4x4						m_WorldMatrix = {};

private:
	class CShader* m_pShaderCom = { nullptr };

private:
	void SetUp_Neighbors();

public:
	static CNavigation_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};
#endif
NS_END
