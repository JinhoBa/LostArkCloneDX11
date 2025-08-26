#include "pch.h"
#include "UIEffect.h"

#include "GameInstance.h"

CUIEffect::CUIEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CUIEffect::CUIEffect(const CUIEffect& Prototype)
	: CUIPanel{ Prototype }, m_fTimeAcc{0.f}
{
}

HRESULT CUIEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIEffect::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	UIEffect_Desc* pDesc = static_cast<UIEffect_Desc*>(pArg);

	UIOBJECT_DESC Desc = {};

	m_fDuration = pDesc->fDuration;

	Desc.fX = pDesc->fX;
	Desc.fY = pDesc->fY;
	Desc.fZ = pDesc->fZ;
	Desc.fSizeX = pDesc->fSizeX;
	Desc.fSizeY = pDesc->fSizeY;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;



	return S_OK;
}

void CUIEffect::Priority_Update(_float fTimeDelta)
{
}

void CUIEffect::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc)
		m_isDead = true;
}

void CUIEffect::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIEffect::Render()
{


	return S_OK;
}

HRESULT CUIEffect::Add_Components()
{
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

void CUIEffect::Free()
{
	__super::Free();
}
