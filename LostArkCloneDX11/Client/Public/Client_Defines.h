#pragma once

#include <process.h>
#include <fstream>
#include "../Default/framework.h"
namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, GAMEPLAY, TUTORIAL, MAP_EDITOR, END};
	enum class STANCE {FLURRY, FOCUS, IDEN_END};
	
	enum class ATTACK_TYPE {NORMAL, HEAD, BACK};
	enum class HIT_TYPE { NORMAL, PUSH, FLOAT };

	enum SKILL_TYPE { SKILL_NORAML, SKILL_COMBO, SKILL_POINT, SKILL_CHARGE };
	enum STAGGER { STAGGER_NONE, STAGGER_LOW, STAGGER_MIDDLE, STAGGER_HIGH };
	enum PART_BREAKER { PART_BREAKER_NONE, PART_BREAKER_LOW, PART_BREAKER_MID, PART_BREAKER_HIGH};
	enum SUPER_ARMOUR {
		SUPRE_ARMOUR_NONE = 0,
		SUPRE_ARMOUR_PARALYSIS = 1 << 1,
		SUPRE_ARMOUR_PUSH = 1 << 2,
		SUPRE_ARMOUR_ALL = SUPRE_ARMOUR_PARALYSIS | SUPRE_ARMOUR_PUSH
	};
	
}

using namespace Client;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;