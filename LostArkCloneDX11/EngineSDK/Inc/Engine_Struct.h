#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE			hInstance;
		HWND				hWnd;
		unsigned int		iNumLevels;
		unsigned int		iWinSizeX, iWinSizeY;
		WINMODE				eWindowMode;
	}ENGINE_DESC;

	typedef struct tagFontDesc
	{
		wstring strWord;
		XMFLOAT2 vPositon;
		XMFLOAT4 vColor;
		float fScale;
	}FONT_DESC;

	typedef struct tagLightDesc
	{
		LIGHT eType;
		XMFLOAT4 vDiffuse;
		XMFLOAT4 vAmbient;
		XMFLOAT4 vSpecular;

		XMFLOAT4 vDirection;
		XMFLOAT4 vPosition;
		float fRange;
	}LIGHT_DESC;

	typedef struct tagShadowLight
	{
		XMFLOAT4 vEye, vAt;
		float	fNear, fFar, fFovy, fAspect;
	}SHADOW_LIGHT_DESC;

	typedef struct tagVertexPosition
	{
		XMFLOAT3			vPosition;

		static constexpr unsigned int					iNumElement = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

		};
	}VTXPOS;

	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3						vPosition;
		XMFLOAT2						vTexcoord;
		static constexpr unsigned int	iNumElement = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXPOSTEX;

	typedef struct tagVertexPositionNomalTexcoord
	{
		XMFLOAT3						vPosition;
		XMFLOAT3						vNormal;
		XMFLOAT2						vTexcoord;
		static constexpr unsigned int	iNumElement = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNORTEX;

	typedef struct tagVertexMesh
	{
		XMFLOAT3						vPosition;
		XMFLOAT3						vNormal;
		XMFLOAT3						vTangent;
		XMFLOAT3						vBinormal;
		XMFLOAT2						vTexcoord;

		static constexpr unsigned int	iNumElement = { 5 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXMESH;

	typedef struct tagVertexAnimMesh
	{
		XMFLOAT3						vPosition;
		XMFLOAT3						vNormal;
		XMFLOAT3						vTangent;
		XMFLOAT3						vBinormal;
		XMFLOAT2						vTexcoord;

		XMUINT4							vBlendIndex;
		XMFLOAT4						vBlendWeight;

		static constexpr unsigned int	iNumElement = { 7 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 68, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXANIMMESH;


	typedef struct tagVertexInstanceParticle
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;

		XMFLOAT2		vLifeTime;
	}VTX_INSTANCE_PARTICLE;

	typedef struct tagVertexInstanceTRAIL
	{
		XMFLOAT4		vStartPosition;
		XMFLOAT4		vEndPosition;
		XMFLOAT4		vStartRight;
		XMFLOAT4		vEndRight;

		XMFLOAT2		vLifeTime;
		XMFLOAT3		vWidth;
	}VTX_INSTANCE_TRAIL;

	typedef struct tagVertexPosTexInstanceParticleDesc
	{
		static constexpr unsigned int		iNumElement = { 7 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 5, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};
	}VTX_POSTEX_INSTANCE_PARTICLE;

	typedef struct tagVertexPosInstanceParticleDesc
	{
		static constexpr unsigned int		iNumElement = { 6 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
	}VTX_POS_INSTANCE_PARTICLE;

	typedef struct tagVertexPosInstanceLineDesc
	{
		static constexpr unsigned int		iNumElement = { 7 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "POSITION", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 72, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
	}VTX_POS_INSTANCE_LINE;

	typedef struct tagKeyFrame
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vTranslation;
		float	 fTrackPosition;
	}KEYFRAME;
}

#endif // Engine_Struct_h__
