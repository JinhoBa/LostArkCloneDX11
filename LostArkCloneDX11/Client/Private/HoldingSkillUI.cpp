#include "pch.h"
#include "HoldingSkillUI.h"

#include "GameManager.h"
#include "GameInstance.h"

#include "Player.h"

CHoldingSkillUI::CHoldingSkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{ pDevice, pContext }, m_pGameManager{ CGameManager::GetInstance()}
{
	Safe_AddRef(m_pGameManager);
}

CHoldingSkillUI::CHoldingSkillUI(const CHoldingSkillUI& Prototype)
	:CUIBar{ Prototype }, m_pGameManager{Prototype.m_pGameManager}
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CHoldingSkillUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHoldingSkillUI::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	UIBAR_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 100.f;
	Desc.fZ = 0.2f;
	Desc.fSizeX = 512.f;
	Desc.fSizeY = 64.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));
	Desc.fMax = 100.f;
	Desc.fStartValue = 80.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pChargeSkill_Desc = dynamic_cast<CPlayer*>(
		m_pGameInstance->Get_LayerObjects(
			ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Get_ChargeSkill_Desc();


	__super::Set_Indices(0, 1);

	m_isActive = false;
	m_fTimeAcc = 0.f;

	return S_OK;
}

void CHoldingSkillUI::Priority_Update(_float fTimeDelta)
{
}

void CHoldingSkillUI::Update(_float fTimeDelta)
{

}

void CHoldingSkillUI::Late_Update(_float fTimeDelta)
{
	if (m_pChargeSkill_Desc->isUsing)
	{
		m_fTimeAcc += fTimeDelta;

		m_SkillName_Font.strWord = to_wstring(m_pChargeSkill_Desc->fChargingTime).substr(0, 3) + L"초";

		m_fValue = m_pChargeSkill_Desc->fChargingTime / m_pChargeSkill_Desc->fMaxChargeTime;

		m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_SkillName_Font);
		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_ChargeTime_Font);
	}
	else
	{
		m_fTimeAcc = 0.f;
	}

	
}

HRESULT CHoldingSkillUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	/*Render Frame*/

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

HRESULT CHoldingSkillUI::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_HoldingSkillUI"),
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

HRESULT CHoldingSkillUI::Ready_Font()
{
	m_SkillName_Font.strWord = L"적령포";
	m_SkillName_Font.vPositon = _float2(m_fX, m_fY);
	m_SkillName_Font.vColor = _float4(0.9f, 0.9f, 0.9f, 1.f);
	m_SkillName_Font.fScale = 0.35f;

	m_ChargeTime_Font.strWord = L"0초";
	m_ChargeTime_Font.vPositon = _float2(m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);
	m_ChargeTime_Font.vColor = _float4(0.9f, 0.9f, 0.9f, 1.f);
	m_ChargeTime_Font.fScale = 0.23f;

	return S_OK;
}

CHoldingSkillUI* CHoldingSkillUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHoldingSkillUI* pInstance = new CHoldingSkillUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CHoldingSkillUI");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHoldingSkillUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CHoldingSkillUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CHoldingSkillUI");
		return nullptr;
	}
	return pInstance;
}

void CHoldingSkillUI::Free()
{
	__super::Free();

	Safe_Release(m_pGameManager);
}
