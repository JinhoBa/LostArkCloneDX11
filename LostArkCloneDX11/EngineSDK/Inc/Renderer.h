#pragma once

/* 객체를 생성시에 그려져야할 객체라면 오브젝트 매니져에도 추가하고, 렌더러에도 추가한다.(x)*/
/* 매 프레임당 객체안에서 그려져야하는가를 판단하고 그려져야한다라면 렌더러에 등록하는 작업을 수행한다. */

/* 화면에 그려져야할 객체들을 그리는 순서대로 분류하여 보관한다. */
/* 보관하고 잇는 객체들을 보관한 순서대로 렌더함수를 호출해 준다. 컨테이너를 클리어해버린다. */
#include "Base.h"

NS_BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	void Toggle_VisibleUI() {
		m_isVisibleUI = !m_isVisibleUI;
	}
	D3D11_VIEWPORT& Get_Veiwport();
	void BlurBackBuffer(_float fStrength);

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void	Render();
	void	Render_Cursor();

#ifdef _DEBUG
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif // _DEBUG

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	_bool						m_isVisibleUI = { true };
	_bool						m_isEnableBlur = { false };
	_uint						m_iPassIndex = {};

	_float						m_fBlurStrength = {};

	_float						m_fMaxDepth = {};
	_float						m_fWinSizeX = {};
	_float						m_fWinSizeY = {};

	list<class CGameObject*>	m_RenderObjects[ENUM_TO_INT(RENDER::END)];

	_float4x4					m_WorldMatrix = {};
	_float4x4					m_OrthographicViewMatrix = {};
	_float4x4					m_OrthographicMatrix = {};

	class CShader*				m_pShaderCom = { nullptr };
	class CVIBuffer_Rect*		m_pVIBufferCom = {};

	D3D11_VIEWPORT				m_Viewport = {};
	_uint						m_iNumViewpprt = { 1 };

	ID3D11DepthStencilView*		m_pShadowDSV = { nullptr };

#ifdef _DEBUG
private:
	list<class CComponent*>		m_DebugComponents;
#endif // _DEBUG

	
private:
	void Render_Priority();
	HRESULT Render_Shadow();
	void Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Blur();
	HRESULT Render_Combined();
	HRESULT Render_BackBufferBlur();
	void Render_NonLight();
	void Render_Blend();
	void Render_WorldUI();
	void Render_UI();

	void Clear_UI();

	void Sort_AlphaObject();
	void Sort_UI();
	HRESULT Ready_DepthStencilView(_uint iSizeX, _uint iSizeY);


#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif // _DEBUG


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

NS_END