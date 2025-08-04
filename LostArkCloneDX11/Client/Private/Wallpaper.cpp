#include "pch.h"
#include "Wallpaper.h"

#include "GameInstance.h"

CWallpaper::CWallpaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CWallpaper::CWallpaper(const CWallpaper& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CWallpaper::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWallpaper::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 100.f;
	Desc.fZ = 0.9f;
	Desc.fSizeX = (_float)g_iWinSizeX;
	Desc.fSizeY = (_float)g_iWinSizeY;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CWallpaper::Priority_Update(_float fTimeDelta)
{
}

void CWallpaper::Update(_float fTimeDelta)
{
}

void CWallpaper::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CWallpaper::Render()
{
	if (FAILED(__super::Bind_ShaderResource(0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWallpaper::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Wallpaper"),
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

CWallpaper* CWallpaper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWallpaper* pInstance = new CWallpaper(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CWallpaper");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWallpaper::Clone(void* pArg)
{
	CGameObject* pInstance = new CWallpaper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CWallpaper");
		return nullptr;
	}
	return pInstance;
}

void CWallpaper::Free()
{
	__super::Free();
}
