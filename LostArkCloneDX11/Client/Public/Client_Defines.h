#pragma once

#include <process.h>
#include <fstream>
#include "../Default/framework.h"

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, TUTORIAL, END};
	enum IDENTITY {FLURRY, FOCUS, IDEN_END};
}

using namespace Client;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;