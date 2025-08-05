#include "pch.h"
#include "HpBar.h"

#include "GameInstance.h"

CHpBar::CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{ pDevice, pContext }
{
}

CHpBar::CHpBar(const CHpBar& Prototype)
	:CUIBar{ Prototype }
{
}

HRESULT CHpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHpBar::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	UIBAR_DESC Desc = {};

	Desc.fX = -194.f;
	Desc.fY = -35.f;
	Desc.fZ = 0.2f;
	Desc.fSizeX = 220.f;
	Desc.fSizeY = 20.f;
	Desc.pParent_TransformCom = static_cast<UIBAR_DESC*>(pArg)->pParent_TransformCom;
	Desc.fMax = 100.f;
	Desc.fStartValue = 80.f;
	Desc.fSizeY_Fill = Desc.fSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CHpBar::Priority_Update(_float fTimeDelta)
{
}

void CHpBar::Update(_float fTimeDelta)
{

}

void CHpBar::Late_Update(_float fTimeDelta)
{
	Update_Position();
	Update_Bar(100.f);
}

HRESULT CHpBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHpBar::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HpBar"),
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

CHpBar* CHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHpBar* pInstance = new CHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CHpBar");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHpBar::Clone(void* pArg)
{
	CGameObject* pInstance = new CHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CHpBar");
		return nullptr;
	}
	return pInstance;
}

void CHpBar::Free()
{
	__super::Free();
}
