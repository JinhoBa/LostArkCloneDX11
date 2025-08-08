#include "pch.h"
#include "AwakeSlot.h"

#include "GameInstance.h"

CAwakeSlot::CAwakeSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CQuickSlot{ pDevice, pContext }
{
}

CAwakeSlot::CAwakeSlot(const CAwakeSlot& Prototype)
	: CQuickSlot{ Prototype }
{
}

HRESULT CAwakeSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAwakeSlot::Initialize(void* pArg)
{
	m_byKey = DIK_V;

	UIOBJECT_DESC Desc= {};

	Desc.fX = -330.f;
	Desc.fY = 25.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CAwakeSlot::Priority_Update(_float fTimeDelta)
{
}

void CAwakeSlot::Update(_float fTimeDelta)
{
	Update_Position();
}

void CAwakeSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAwakeSlot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAwakeSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_AwakeTexture"), reinterpret_cast<CComponent**>(&m_pAwakeTextureCom))))
		return E_FAIL;

	return S_OK;
}

CAwakeSlot* CAwakeSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAwakeSlot* pInstance = new CAwakeSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAwakeSlot");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CAwakeSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CAwakeSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CAwakeSlot");
		return nullptr;
	}
	return pInstance;
}

void CAwakeSlot::Free()
{
	__super::Free();

	Safe_Release(m_pAwakeTextureCom);
}
