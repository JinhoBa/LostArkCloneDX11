#include "pch.h"
#include "ExpBar.h"

#include "GameInstance.h"

CExpBar::CExpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{ pDevice, pContext }
{
}

CExpBar::CExpBar(const CExpBar& Prototype)
	:CUIBar{ Prototype }
{
}

HRESULT CExpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExpBar::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	UIBAR_DESC Desc = {};

	Desc.fX = 43.f;
	Desc.fY = 358.f;
	Desc.fZ = 0.1f;
	Desc.fSizeX = 1368.f;
	Desc.fSizeY = 12.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));
	Desc.fMax = 100.f;
	Desc.fStartValue = 0.1f;
	Desc.fSizeY_Fill = 7.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CExpBar::Priority_Update(_float fTimeDelta)
{
}

void CExpBar::Update(_float fTimeDelta)
{

}

void CExpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);


	Update_Bar(50.f);
	Set_Indices(0, 1);
}

HRESULT CExpBar::Render()
{
	// UI
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	// Background
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	// Render Fill
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransfromCom_BarFill->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iSRVIndex_Fill))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	/* Render Back */ 
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iSRVIndex_Back))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExpBar::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ExpBar"),
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

	/*Transform_Fill*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Fill"), reinterpret_cast<CComponent**>(&m_pTransfromCom_BarFill))))
		return E_FAIL;

	return S_OK;
}

HRESULT CExpBar::Draw()
{
	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CExpBar* CExpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExpBar* pInstance = new CExpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CExpBar");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CExpBar::Clone(void* pArg)
{
	CGameObject* pInstance = new CExpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CExpBar");
		return nullptr;
	}
	return pInstance;
}

void CExpBar::Free()
{
	__super::Free();
}
