#include "Renderer.h"

#include "GameInstance.h"

#include "UIObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() },
	m_isEnableBlur{ false }
{
    Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

D3D11_VIEWPORT& CRenderer::Get_Veiwport()
{
	m_pContext->RSGetViewports(&m_iNumViewpprt, &m_Viewport);

	return  m_Viewport;
}

void CRenderer::BlurBackBuffer(_float fStrength)
{
	m_fBlurStrength = fStrength;

	m_isEnableBlur = true;
}

HRESULT CRenderer::Initialize()
{
	m_iPassIndex = 3;

	m_pContext->RSGetViewports(&m_iNumViewpprt, &m_Viewport);

	m_fWinSizeX = m_Viewport.Width;
	m_fWinSizeY = m_Viewport.Height;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Emissive"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_RimLight"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shadow"), g_iMaxWidth, g_iMaxHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilView(g_iMaxWidth, g_iMaxHeight)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_X"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Emissive"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BackBuffer"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	m_fMaxDepth = m_Viewport.MaxDepth;

	/* MRT GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Emissive"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_RimLight"))))
		return E_FAIL;

	/* MRT LightAcc */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	/* MRT_Shadow */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))
		return E_FAIL;


	/* MRT_Blur_X */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur"), TEXT("Target_Blur"))))
		return E_FAIL;

	/* MRT_Blur_X */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
		return E_FAIL;

	/* MRT_Blur_X */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Emissive"), TEXT("Target_Blur_Emissive"))))
		return E_FAIL;

	/* MRT_Blur_BACKBUFFER */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BackBuffer"), TEXT("Target_BackBuffer"))))
		return E_FAIL;

	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"),
		VTXPOSTEX::Elements, VTXPOSTEX::iNumElement);
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pVIBufferCom = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(m_Viewport.Width, m_Viewport.Height, 1.f));
	XMStoreFloat4x4(&m_OrthographicViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_OrthographicMatrix, (XMMatrixOrthographicLH(m_Viewport.Width, m_Viewport.Height, 0.f, 1.f)));

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Shade"), 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Normal"), 150.f, 450.f, 300.f, 300.f)))
	//	return E_FAIL;
	/*if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_RimLight"), 75.f, 225.f, 150.f, 150.f)))
		return E_FAIL;*/
	//if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Shadow"), 1280.f - 150.f, 150.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Specular"), 225.f, 225.f, 150.f, 150.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Blur_X"), 75.f, 375.f, 150.f, 150.f)))
	//	return E_FAIL;
	
#endif // _DEBUG

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
	Sort_UI();

	Render_Priority();

	Render_NonBlend();

	if (FAILED(Render_Shadow()))
		MSG_BOX("Failed to Render Shadow");

	if (FAILED(Render_LightAcc()))
	{
		MSG_BOX("Failed to Render LightAcc");
	}

	if (FAILED(Render_Blur()))
	{
		MSG_BOX("Failed to Render Blur");
	}

	if (FAILED(Render_Combined()))
	{
		MSG_BOX("Failed to Render Combined");
	}

	if (m_isEnableBlur)
	{
		Render_BackBufferBlur();
	}

	Render_NonLight();
	Render_Blend();
	
	if(true == m_isVisibleUI)
	{
		Render_WorldUI();
		Render_UI();
	}
	else
	{
		Clear_UI();
	}

#ifdef _DEBUG
	Render_Debug();
#endif // _DEBUG

}

HRESULT CRenderer::Add_DebugComponent(CComponent* pDebugCom)
{
	m_DebugComponents.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}

void CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::PRIORITY)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::PRIORITY)].clear();
}
HRESULT	 CRenderer::Render_Shadow()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), m_pShadowDSV)))
		return E_FAIL;

	D3D11_VIEWPORT ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	ViewPortDesc.TopLeftX = 0.f;
	ViewPortDesc.TopLeftY = 0.f;
	ViewPortDesc.Width = static_cast<_float>(g_iMaxWidth);
	ViewPortDesc.Height = static_cast<_float>(g_iMaxHeight);
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::SHADOW)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render_Shadow();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::SHADOW)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	ViewPortDesc.TopLeftX = 0.f;
	ViewPortDesc.TopLeftY = 0.f;
	ViewPortDesc.Width = m_fWinSizeX;
	ViewPortDesc.Height = m_fWinSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

void CRenderer::Render_NonBlend()
{
	/* Diffuse, Normal, Emissive */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return;

	HRESULT hr = {};
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::NONBLEND)])
	{
		if (nullptr != pRenderObject)
			hr = pRenderObject->Render();

		if (hr == E_FAIL)
			MSG_BOX("Failed Render");
		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::NONBLEND)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

HRESULT CRenderer::Render_LightAcc()
{
	if(FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix);
	m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::PROJ));
	m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4));
	m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float));


	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Normal"), m_pShaderCom, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	if(FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Lights(m_pShaderCom, m_pVIBufferCom)))
		return E_FAIL;

	return m_pGameInstance->End_MRT();
}

HRESULT CRenderer::Render_Blur()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::BLUR)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::BLUR)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Blur"), m_pShaderCom, "g_BlurTexture")))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	/* Emissive */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Emissive"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Emissive"), m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;

	m_pShaderCom->Begin(7);

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Combined()
{
	if (m_isEnableBlur)
	{
		if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BackBuffer"))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far(), sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::PROJ));

	if (FAILED(m_pGameInstance->Bind_Shadow_Resource(m_pShaderCom, "g_LightViewMatrix", D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_Shadow_Resource(m_pShaderCom, "g_LightProjMatrix", D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Diffuse"), m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Shade"), m_pShaderCom, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Specular"), m_pShaderCom, "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Shadow"), m_pShaderCom, "g_ShadowTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Blur_Emissive"), m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Blur_X"), m_pShaderCom, "g_BlurHorizontalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_RimLight"), m_pShaderCom, "g_RimLightTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_isEnableBlur)
	{
		if (FAILED(m_pGameInstance->End_MRT()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Render_BackBufferBlur()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlurStrength", &m_fBlurStrength, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_BackBuffer"), m_pShaderCom, "g_BlurTexture")))
		return E_FAIL;

	m_pShaderCom->Begin(5);

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlurStrength", &m_fBlurStrength, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_BackBuffer"), m_pShaderCom, "g_BackBufferTexture")))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_isEnableBlur = false;

	return S_OK;
}

void CRenderer::Render_NonLight()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::NONLIGHT)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::NONLIGHT)].clear();
}

void CRenderer::Render_Blend()
{
	m_RenderObjects[ENUM_TO_INT(RENDER::BLEND)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool {
		return pSour->Get_Depth() > pDest->Get_Depth();
		});

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::BLEND)].clear();
}

void CRenderer::Render_WorldUI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::WORLDUI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::WORLDUI)].clear();
}

void CRenderer::Render_UI()
{
	Sort_UI();

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::UI)].clear();
}

void CRenderer::Clear_UI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::WORLDUI)])
		Safe_Release(pRenderObject);

	m_RenderObjects[ENUM_TO_INT(RENDER::WORLDUI)].clear();

	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::UI)])
		Safe_Release(pRenderObject);

	m_RenderObjects[ENUM_TO_INT(RENDER::UI)].clear();
}

void CRenderer::Render_Cursor()
{
	if(m_isVisibleUI)
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_INT(RENDER::CURSOR)])
	{
		if (nullptr != pRenderObject && true == m_isVisibleUI)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_TO_INT(RENDER::CURSOR)].clear();
}

void CRenderer::Sort_AlphaObject()
{
	
}

void CRenderer::Sort_UI()
{
	m_RenderObjects[ENUM_TO_INT(RENDER::UI)].sort(
		[](CGameObject* pSrc, CGameObject* pDst)->_bool {
			return dynamic_cast<CUIObject*>(pSrc)->Get_ZValue() > dynamic_cast<CUIObject*>(pDst)->Get_ZValue();});
}

HRESULT CRenderer::Ready_DepthStencilView(_uint iSizeX, _uint iSizeY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	/*Render RenderTargets */

	for (auto& pDebugCom : m_DebugComponents)
	{
		if (nullptr != pDebugCom)
			pDebugCom->Render();
		
		Safe_Release(pDebugCom);
	}
	m_DebugComponents.clear();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_RenderTarget_Debug(TEXT("MRT_GameObjects"), m_pShaderCom, m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RenderTarget_Debug(TEXT("MRT_LightAcc"), m_pShaderCom, m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RenderTarget_Debug(TEXT("MRT_Shadow"), m_pShaderCom, m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}
#endif // _DEBUG

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

	for (auto& pComponent : m_DebugComponents)
		Safe_Release(pComponent);
	m_DebugComponents.clear();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}

	Safe_Release(m_pShadowDSV);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
