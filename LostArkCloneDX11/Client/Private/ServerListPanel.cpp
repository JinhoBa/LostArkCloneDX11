#include "pch.h"
#include "ServerListPanel.h"

#include "GameInstance.h"

CServerListPanel::CServerListPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{pDevice, pContext}
{
}

CServerListPanel::CServerListPanel(const CServerListPanel& Prototype)
	: CUIPanel{Prototype}
{
}

HRESULT CServerListPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CServerListPanel::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 100.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 340;
	Desc.fSizeY = 340;
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

void CServerListPanel::Priority_Update(_float fTimeDelta)
{
}

void CServerListPanel::Update(_float fTimeDelta)
{
}

void CServerListPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CServerListPanel::Render()
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

HRESULT CServerListPanel::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_ServerListBack"),
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

CServerListPanel* CServerListPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CServerListPanel* pInstance = new CServerListPanel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CServerListPanel");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CServerListPanel::Clone(void* pArg)
{
	CGameObject* pInstance = new CServerListPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CServerListPanel");
		return nullptr;
	}
	return pInstance;
}

void CServerListPanel::Free()
{
	__super::Free();
}
