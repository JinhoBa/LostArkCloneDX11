#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CLoader final : public CBase
{
public:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();
	_float	Output();

public:
	_bool isFinished() { return m_isFinished; }

private:
	_bool					m_isFinished = { false };
	LEVEL					m_eNextLevelID = { LEVEL::END };

	_float					m_fLoadProgress = {};
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };
	
	HANDLE					m_hThread = {};

	_wstring				m_strMessage = {};
	CRITICAL_SECTION		m_Critical_Section = {};

private:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	HRESULT Loading_For_Tutorial();
	HRESULT Loading_For_MapEditor();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

NS_END