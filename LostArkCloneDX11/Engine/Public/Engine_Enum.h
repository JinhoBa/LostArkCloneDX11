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
}
#endif // Engine_Enum_h__
