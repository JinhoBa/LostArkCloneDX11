#include "pch.h"
#include "QuickSlot.h"

#include "GameInstance.h"

CQuickSlot::CQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CQuickSlot::CQuickSlot(const CQuickSlot& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CQuickSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuickSlot::Initialize(void* pArg)
{
	QUICKSLOT_DESC* pDesc = static_cast<QUICKSLOT_DESC*>(pArg);

	m_byKey = pDesc->byKey;
	m_iSlotID = pDesc->iSlotID;

	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CQuickSlot::Priority_Update(_float fTimeDelta)
{
}

void CQuickSlot::Update(_float fTimeDelta)
{

}

void CQuickSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CQuickSlot::Render()
{
	if (FAILED(Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pFrameTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iTextureIndex))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pFrameTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuickSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_FrameTexture"), reinterpret_cast<CComponent**>(&m_pFrameTextureCom))))
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

HRESULT CQuickSlot::Bind_Resource()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuickSlot::Draw()
{
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CQuickSlot* CQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CQuickSlot* pInstance = new CQuickSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CUIPanel");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CQuickSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CQuickSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CUIPanel");
		return nullptr;
	}

	return pInstance;
}

void CQuickSlot::Free()
{
	__super::Free();

	Safe_Release(m_pFrameTextureCom);
}
