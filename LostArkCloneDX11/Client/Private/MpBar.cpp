#include "pch.h"
#include "MpBar.h"

#include "GameInstance.h"

CMpBar::CMpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{ pDevice, pContext }
{
}

CMpBar::CMpBar(const CMpBar& Prototype)
	:CUIBar{ Prototype }
{
}

HRESULT CMpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMpBar::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	UIBAR_DESC Desc = {};

	Desc.fX = 194.f;
	Desc.fY = -35.f;
	Desc.fZ = 0.2f;
	Desc.fSizeX = 220.f;
	Desc.fSizeY = 20.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_HUD"), TEXT("Com_Transform"),0
	));
	Desc.fMax = 100.f;
	Desc.fStartValue = 80.f;
	Desc.fSizeY_Fill = Desc.fSizeY;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CMpBar::Priority_Update(_float fTimeDelta)
{
}

void CMpBar::Update(_float fTimeDelta)
{

}

void CMpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	Update_Position();
	Update_Bar(100.f);

}

HRESULT CMpBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMpBar::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MpBar"),
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

CMpBar* CMpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMpBar* pInstance = new CMpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMpBar");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMpBar::Clone(void* pArg)
{
	CGameObject* pInstance = new CMpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CMpBar");
		return nullptr;
	}
	return pInstance;
}

void CMpBar::Free()
{
	__super::Free();
}
