#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();
	
	POINT& Get_MousePoint();
	_float3& Get_MousePosition(RAY eSpace);

	void Transform_ToLocalSpace(const FXMMATRIX pWorldMatrixInverse);

	_bool Picking_InWorldSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut);
	_bool Picking_InLocalSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	HWND					m_hWnd = {};
	_float					m_fWinSizeX{}, m_fWinSizeY{};

	POINT					m_ptMouse = {};
	_float3					m_vRayDir[ENUM_TO_INT(RAY::END)];
	_float3					m_vRayPos[ENUM_TO_INT(RAY::END)];

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;

};

NS_END