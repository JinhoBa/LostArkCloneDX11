#include "pch.h"
#include "ActiveSlot.h"

#include "GameInstance.h"

CActiveSlot::CActiveSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CQuickSlot{ pDevice, pContext }
{
}

CActiveSlot::CActiveSlot(const CActiveSlot& Prototype)
	: CQuickSlot{ Prototype }
{
}

HRESULT CActiveSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CActiveSlot::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CActiveSlot::Priority_Update(_float fTimeDelta)
{
}

void CActiveSlot::Update(_float fTimeDelta)
{

}

void CActiveSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CActiveSlot::Render()
{
	if(m_isVisible)
	{

		if (DIK_1 == m_byKey)
		{
			if (FAILED(__super::Bind_Resource()))
				return E_FAIL;

			if (FAILED(Render_SlotBack()))
				return E_FAIL;
			
			if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pItemTextureCom->Get_SRV(0))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Bind_Resources()))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;

			if (FAILED(Render_SlotFront()))
				return E_FAIL;
		}
		else
		{
			if (FAILED(__super::Render()))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CActiveSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_BattleItem"),
		TEXT("Com_ItmeTexture"), reinterpret_cast<CComponent**>(&m_pItemTextureCom))))
		return E_FAIL;


	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CActiveSlot* CActiveSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CActiveSlot* pInstance = new CActiveSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CActiveSlot");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CActiveSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CActiveSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CActiveSlot");
		return nullptr;
	}
	return pInstance;
}

void CActiveSlot::Free()
{
	__super::Free();

	Safe_Release(m_pItemTextureCom);
}
