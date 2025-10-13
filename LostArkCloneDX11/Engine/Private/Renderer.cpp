#include "Renderer.h"

#include "GameInstance.h"

#include "UIObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{CGameInstance::GetInstance()}
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

HRESULT CRenderer::Initialize()
{
	m_iPassIndex = 3;

	m_pContext->RSGetViewports(&m_iNumViewpprt, &m_Viewport);

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Emissive"), (_uint)m_Viewport.Width, (_uint)m_Viewport.Height,
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

	/* MRT_LightAcc */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
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
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Diffuse"), 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Normal"), 50.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Shade"), 150.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Specular"), 150.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RenderTarget_Debug(TEXT("Target_Emissive"), 50.f, 250.f, 100.f, 100.f)))
		return E_FAIL;
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

	if (FAILED(Render_LightAcc()))
	{
		MSG_BOX("Failed to Render LightAcc");
	}

	if (FAILED(Render_Combined()))
	{
		MSG_BOX("Failed to Render Combined");
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
	m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transfrom_Float4x4_Inverse(D3DTS::VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transfrom_Float4x4_Inverse(D3DTS::PROJ));
	m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4));
	
	m_pContext->RSGetViewports(&m_iNumViewpprt, &m_Viewport);

	m_fMaxDepth = m_Viewport.MaxDepth;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", &m_fMaxDepth, sizeof(_float))))
		return E_FAIL;

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

HRESULT CRenderer::Render_Combined()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_OrthographicViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_OrthographicMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Diffuse"), m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Shade"), m_pShaderCom, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Specular"), m_pShaderCom, "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Emissive"), m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	return m_pVIBufferCom->Render();
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

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
