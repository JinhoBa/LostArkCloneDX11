#include "pch.h"
#include "LoadingBar.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Loading.h"

CLoadingBar::CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{pDevice, pContext}
{
}

CLoadingBar::CLoadingBar(const CLoadingBar& Prototype)
	:CUIBar{Prototype}
{
}

HRESULT CLoadingBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingBar::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
		
	m_pLoading = static_cast<CLevel_Loading*>(pArg);

	Safe_AddRef(m_pLoading);

	if (FAILED(Add_Components()))
		return E_FAIL;

	UIBAR_DESC Desc = {};

	Desc.fX = -10.f;
	Desc.fY = 300.f;
	Desc.fZ = 0.2f;
	Desc.fSizeX = (_float)g_iWinSizeX;
	Desc.fSizeY = 10.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));
	Desc.fMax = 100.f;
	Desc.fSizeY_Fill = Desc.fSizeY;
	Desc.fStartValue = 0.1f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pTransformCom_Point->Set_Scale(_float3(340.f, 30.f, 1.f));
	return S_OK;
}

void CLoadingBar::Priority_Update(_float fTimeDelta)
{
}

void CLoadingBar::Update(_float fTimeDelta)
{
	_float fProgress = m_pLoading->Get_Progress();

	Update_Bar(fProgress);

	_vector vPoint = XMVectorSet(fProgress* 12.2f - 720.f, -300.f, 1.f, 1.f);

	m_pTransformCom_Point->Set_State(STATE::POSITION, vPoint);

	m_fValue = fProgress / 100.f;
	
}

void CLoadingBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

	Set_Indices(0, 1);
}

HRESULT CLoadingBar::Render()
{
	__super::Render();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Point->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBar::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBar"),
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

	/*Transform_Fill*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Point"), reinterpret_cast<CComponent**>(&m_pTransformCom_Point))))
		return E_FAIL;

	return S_OK;
}

CLoadingBar* CLoadingBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoadingBar* pInstance = new CLoadingBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CLoadingBar");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CLoadingBar::Clone(void* pArg)
{
	CGameObject* pInstance = new CLoadingBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CLoadingBar");
		return nullptr;
	}
	return pInstance;
}

void CLoadingBar::Free()
{
	__super::Free();

	Safe_Release(m_pLoading);
	Safe_Release(m_pTransformCom_Point);
}
