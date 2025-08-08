#ifndef Engine_Define_h__
#define Engine_Define_h__

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#include "DirectXTM/DDSTextureLoader.h"
#include "DirectXTM/WICTextureLoader.h"
#include "FX11/d3dx11effect.h"

using namespace DirectX;

#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

namespace Engine
{
	const static wstring strTransformTag = { TEXT("Com_Transform") };
}
#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


using namespace Engine;


#endif // Engine_Define_h__
