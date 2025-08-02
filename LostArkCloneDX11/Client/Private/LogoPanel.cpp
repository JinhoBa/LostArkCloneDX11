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
	Desc.fY = -200.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 590.f;
	Desc.fSizeY = 340.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

#pragma region TEST_CODE

	_float4x4 CameraWorldInv = {};
	_float4x4 OrthMatrix = {};

	XMStoreFloat4x4(&CameraWorldInv, XMMatrixIdentity());
	XMStoreFloat4x4(&OrthMatrix, (XMMatrixOrthographicLH(static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY), 0.1f, 10.f)));

	m_pShaderCom->Set_Matrix(m_pTransformCom->Get_WorldMatrix(), CameraWorldInv, OrthMatrix);
	m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(0));
#pragma endregion


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

	Update_Position();

	m_pShaderCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

	m_pShaderCom->SetResource(m_pTextureCom->Get_SRV(0));

	if (FAILED(m_pShaderCom->Apply()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources(m_pShaderCom->Get_Layout())))
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

	CShader::SHADER_DESC Shader_Desc = {};
	Shader_Desc.szPassName = "AlphaTestPass";
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
		TEXT("Com_Shader_VTXPosTex"), reinterpret_cast<CComponent**>(&m_pShaderCom), &Shader_Desc)))
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
