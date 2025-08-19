#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CGameManager;
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize();
	void		Update(_float fTimeDelta);
	HRESULT		Render();

private:
	_int					m_iFrame = {};
	_int					m_iFps = {};
	_float					m_fTimeAcc = {};
	CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameManager*			m_pGameManager = { nullptr };
	

private:
	HRESULT Start_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype();
	HRESULT Ready_Layer_Canvars();
	HRESULT Ready_Layer_Mouse();
	HRESULT Ready_Font();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

NS_END