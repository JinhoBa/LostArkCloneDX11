#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class  CInput_Device final : public CBase
{
private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byKeyState[byKeyID];
	}

	_bool	Get_KeyDown(_ubyte byKeyID)
	{
		return (m_byKeyState[byKeyID] & 0x80) && !(m_byPreKeyState[byKeyID] & 0x80);
	}
	_bool	Get_KeyUp(_ubyte byKeyID)
	{
		return !(m_byKeyState[byKeyID] & 0x80) && (m_byPreKeyState[byKeyID] & 0x80);
	}
	_bool	Get_KeyPressing(_ubyte byKeyID)
	{
		return (m_byKeyState[byKeyID] & 0x80) && (m_byPreKeyState[byKeyID] & 0x80);
	}


	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)];
	}

	_bool	Get_DIMouseDown(MOUSEKEYSTATE eMouse)
	{
		return (m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80) && !(m_tPreMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80);
	}
	_bool	Get_DIMouseUp(MOUSEKEYSTATE eMouse)
	{
		return !(m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80) && (m_tPreMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80);
	}
	_bool	Get_DIMousePressing(MOUSEKEYSTATE eMouse)
	{
		return (m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80) && (m_tPreMouseState.rgbButtons[static_cast<_uint>(eMouse)] & 0x80);
	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *((reinterpret_cast<_int*>(&m_tMouseState)) + static_cast<_uint>(eMouseState));
	}

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update();

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pJoystick = { nullptr };

private:
	_byte					m_byPreKeyState[256] = {};
	_byte					m_byKeyState[256] = {};

	DIMOUSESTATE			m_tPreMouseState = {};
	DIMOUSESTATE			m_tMouseState = {};

public:
	static CInput_Device* Create(HINSTANCE hInstance, HWND hWnd);
	virtual void Free(void);
};

NS_END



