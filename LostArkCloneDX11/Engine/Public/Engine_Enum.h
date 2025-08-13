#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE { FULL, WIN, END };	
	enum class PROTOTYPE { GAMEOBJECT, COMPONENT };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	enum class RENDER { PRIORITY, NONBLEND, BLEND, UI, END};
	enum class TEXTURE { PLANE, CUBE, END };
	enum class RAY { LOCAL, WORLD, END };
	enum class CHANNELID { BGM, EFFECT, END };
	enum class D3DTS {VIEW, PROJ, END};
	enum class MOUSEKEYSTATE { LBUTTON, RBUTTON, WBUTTON, XBUTTON };
	enum class MOUSEMOVESTATE { X, Y, W };
	enum class MOUSESPACE { VIEWPORT, PROJ, VIEW, WORLD, LOCAL };
}
#endif // Engine_Enum_h__
