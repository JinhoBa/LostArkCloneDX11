#ifndef Engine_Define_h__
#define Engine_Define_h__

#define DIRECTINPUT_VERSION	0x0800

#include <d3d11.h>
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#include "DirectXTM/DDSTextureLoader.h"
#include "DirectXTM/WICTextureLoader.h"
#include "DirectXTM/SpriteBatch.h"
#include "DirectXTM/SpriteFont.h"
#include "FX11/d3dx11effect.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include <sstream>
#include <charconv>

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
