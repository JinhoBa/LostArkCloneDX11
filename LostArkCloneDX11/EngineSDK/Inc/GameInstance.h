#pragma once

#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void	Update_Engine(_float fTimeDelta);
	HRESULT Draw();
	void	Clear_Resources(_uint iLevelIndex);

	_float Random_Normal();
	_wstring Utf8ToWstring(const char* pStr);
	_float Random(_float fMin, _float fMax);
	_float2& Get_WinSize() { return m_vWinSize; }

#pragma region GRAPHIC_DEVICE
public:
	void Render_Begin(const _float4* pClearColor);
	void Render_End();
	
#pragma endregion

#pragma region TIMER_MANAGER
	_float	Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void	Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region INPUT
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_bool	Get_KeyDown(_ubyte byKeyID);
	_bool	Get_KeyUp(_ubyte byKeyID);
	_bool	Get_KeyPressing(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseDown(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseUp(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMousePressing(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT			Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*	Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	CComponent*					Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	HRESULT						Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	list<class CGameObject*>*	Get_LayerObjects(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region SOUND_MANAGER
	void Play_Sound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region PIPELINE
	void				Set_Transform(D3DTS eState, _fmatrix Matrix);
	const _float4x4*	Get_Transfrom_Float4x4(D3DTS eState) const;
	_matrix				Get_Transfrom_Matrix(D3DTS eState);
	_matrix				Get_Transfrom_MatrixInverse(D3DTS eState);
	const _float4*		Get_Camera_Position() const;
#pragma endregion

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*		m_pPrototype_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CSound_Manager*			m_pSound_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPicking*					m_pPicking = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };

	_float2							m_vWinSize = {};
	
public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END