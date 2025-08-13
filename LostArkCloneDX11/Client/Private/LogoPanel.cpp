#include "pch.h"
#include "LogoPanel.h"

#include "GameInstance.h"

CLogoPanel::CLogoPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CLogoPanel::CLogoPanel(const CLogoPanel& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CLogoPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogoPanel::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = -340.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 610.f;
	Desc.fSizeY = 320.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CLogoPanel::Priority_Update(_float fTimeDelta)
{
}

void CLogoPanel::Update(_float fTimeDelta)
{
}

void CLogoPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CLogoPanel::Render()
{
	if (FAILED(__super::Bind_ShaderResource(0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoPanel::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
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

CLogoPanel* CLogoPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLogoPanel* pInstance = new CLogoPanel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CLogoPanel");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CLogoPanel::Clone(void* pArg)
{
	CGameObject* pInstance = new CLogoPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CLogoPanel");
		return nullptr;
	}
	return pInstance;
}

void CLogoPanel::Free()
{
	__super::Free();
}
