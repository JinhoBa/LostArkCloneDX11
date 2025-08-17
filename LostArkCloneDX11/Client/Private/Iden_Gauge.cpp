#include "pch.h"
#include "Iden_Gauge.h"

#include "GameInstance.h"

CIden_Gauge::CIden_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CIden_Gauge::CIden_Gauge(const CIden_Gauge& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CIden_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIden_Gauge::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	UIOBJECT_DESC Desc = {};

	Desc.fX = 3.f;
	Desc.fY = 14.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 115.f;
	Desc.fSizeY = 115.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;



	return S_OK;
}

void CIden_Gauge::Priority_Update(_float fTimeDelta)
{
}

void CIden_Gauge::Update(_float fTimeDelta)
{

}

void CIden_Gauge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CIden_Gauge::Render()
{
#pragma region TEST_CODE
	Update_Position();
	ImGui::InputFloat("X", &m_fRX, 1.f, 10.f);
	ImGui::InputFloat("Y", &m_fRY, 1.f, 10.f);
	ImGui::InputFloat("SIZE X", &m_fSizeX, 1.f, 10.f);
	ImGui::InputFloat("SIZE Y", &m_fSizeY, 1.f, 10.f);
#pragma endregion
	if (FAILED(__super::Bind_ShaderResource(1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIden_Gauge::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Gauge"),
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

CIden_Gauge* CIden_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIden_Gauge* pInstance = new CIden_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIden_Gauge");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CIden_Gauge::Clone(void* pArg)
{
	CGameObject* pInstance = new CIden_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CIden_Gauge");
		return nullptr;
	}
	return pInstance;
}

void CIden_Gauge::Free()
{
	__super::Free();
}
