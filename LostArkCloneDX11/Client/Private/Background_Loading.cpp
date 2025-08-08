#include "pch.h"
#include "Background_Loading.h"

#include "GameInstance.h"

CBackground_Loading::CBackground_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBackground_Loading::CBackground_Loading(const CBackground_Loading& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CBackground_Loading::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackground_Loading::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.fZ = 0.7f;
	Desc.fSizeX = (_float)g_iWinSizeX;
	Desc.fSizeY = (_float)g_iWinSizeY;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

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

	return S_OK;
}

void CBackground_Loading::Priority_Update(_float fTimeDelta)
{
}

void CBackground_Loading::Update(_float fTimeDelta)
{
}

void CBackground_Loading::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CBackground_Loading::Render()
{
	//m_pContext->OMSetBlendState(m_pBlendState, m_fFactor, 0xffffffff);

	if (FAILED(__super::Bind_ShaderResource(0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	//m_pContext->OMSetBlendState(nullptr, m_fFactor, 0xffffffff);

	return S_OK;
}

HRESULT CBackground_Loading::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*VIBuffer_Rect*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/*Shader_VTXPosTex*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
		TEXT("Com_Shader_VTXPosTex"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	return S_OK;
}

CBackground_Loading* CBackground_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackground_Loading* pInstance = new CBackground_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBackground_Loading");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBackground_Loading::Clone(void* pArg)
{
	CGameObject* pInstance = new CBackground_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBackground_Loading");
		return nullptr;
	}
	return pInstance;
}

void CBackground_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pBlendState);
}
