#pragma once

#include <process.h>
#include <fstream>
#include "../Default/framework.h"
#include "Client_Struct.h"
namespace Client
{
	const unsigned int g_iWinSizeX = 1920;
	const unsigned int g_iWinSizeY = 1080;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, TUTORIAL, MAP_EDITOR, END};
	enum STANCE {FLURRY, FOCUS, IDEN_END};
	enum ATTACK_TYPE {FRONT, BACK};
	enum WEEK_POINT {LOW, MID, HIGH};
}

using namespace Client;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;