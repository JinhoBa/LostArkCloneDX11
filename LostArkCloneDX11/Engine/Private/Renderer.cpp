#include "Renderer.h"

#include "GameObject.h"
#include "UIObject.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	D3D11_VIEWPORT Viewport = {};
	_uint i = { 1 };
	m_pContext->RSGetViewports(&i, &Viewport);

	XMStoreFloat4x4(&m_OrthographicViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_OrthographicMatrix, (XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.1f, 10.f)));


#pragma region DEPTH_STENCIL_STATE
	D3D11_DEPTH_STENCIL_DESC NoDepth_Desc = {};
	NoDepth_Desc.DepthEnable = false;

	D3D11_DEPTH_STENCIL_DESC DS_Priority_Desc = {};
	DS_Priority_Desc.DepthEnable = false;
	DS_Priority_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	if (FAILED(m_pDevice->CreateDepthStencilState(&NoDepth_Desc, &m_pDSState_UI)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilState(&DS_Priority_Desc, &m_pDSState_Priority)))
		return E_FAIL;
#pragma endregion

#pragma region BLEND_STATE
	D3D11_BLEND_DESC Blend_Desc = {};

	Blend_Desc.AlphaToCoverageEnable = false;
	Blend_Desc.IndependentBlendEnable = false;
	Blend_Desc.RenderTarget[0].BlendEnable = true;
	Blend_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Blend_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Blend_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Blend_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Blend_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	Blend_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Blend_Desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;


	if (FAILED(m_pDevice->CreateBlendState(&Blend_Desc, &m_pBlendState)))
		return E_FAIL;

	ZeroMemory(m_fFactor, sizeof(_float) * 4);
#pragma endregion

    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[ENUM_TO_INT(eRenderGroup)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

void CRenderer::Render()
{
	Render_Priority();
	Render_NonBlend();
	Render_Blend();
	Render_UI();
}

void CRenderer::Render_Priority()
{
	m_pContext->OMSetDepthStencilState(m_pDSState_Priority, 1);

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::PRIORITY)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::PRIORITY)].clear();

	m_pContext->OMSetDepthStencilState(nullptr, 1);
}

void CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::NONBLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::NONBLEND)].clear();
}

void CRenderer::Render_Blend()
{
	m_pContext->OMSetBlendState(m_pBlendState, m_fFactor, 0xffffffff);

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::BLEND)].clear();

	m_pContext->OMSetBlendState(nullptr, m_fFactor, 0xffffffff);
}

void CRenderer::Render_UI()
{
	// ±íÀÌ Å×½ºÆ® ²ô±â
	m_pContext->OMSetDepthStencilState(m_pDSState_UI, 1);

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::UI)].clear();

	m_pContext->OMSetDepthStencilState(nullptr, 1);
}

void CRenderer::Sort_AlphaObject()
{
	
}

void CRenderer::Sort_UI()
{
	m_RenderObjects[ENUM_TO_INT(RENDER::UI)].sort(
		[](CGameObject* pSrc, CGameObject* pDst)->_bool {
			return dynamic_cast<CUIObject*>(pSrc)->Get_ZValue() < dynamic_cast<CUIObject*>(pDst)->Get_ZValue();});
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
    __super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pDSState_Priority);
	Safe_Release(m_pDSState_UI);

}
