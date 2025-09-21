#include "pch.h"
#include "BossHpBar.h"

#include "GameInstance.h"

#include "Kamen.h"

CBossHpBar::CBossHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar{ pDevice, pContext }
{
}

CBossHpBar::CBossHpBar(const CBossHpBar& Prototype)
	:CUIBar{ Prototype }
{
}

HRESULT CBossHpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossHpBar::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	CKamen* pKamen = dynamic_cast<CKamen*>(
		m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Kamen")).back());

	if (nullptr == pKamen)
		return E_FAIL;

	m_pKamenInfo = pKamen->Get_InfoPtr();

	UIOBJECT_DESC* pParent_Desc = static_cast<UIOBJECT_DESC*>(pArg);

	UIBAR_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.fZ = 0.1f;
	Desc.fSizeX = 523.f;
	Desc.fSizeY = 28.f;
	Desc.pParent_TransformCom = pParent_Desc->pParent_TransformCom;
	Desc.fMax = 10000000.f;
	Desc.fStartValue = 0.1f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	Set_Indices(0, 1);

	m_fValue = (_float)((_uint)m_pKamenInfo->fHp % 10000000) / 10000000.f;

	m_iFrontIndex = (_uint)(m_pKamenInfo->fHp / 10000000.f) / 7;
	m_iBackIndex = m_iFrontIndex + 1;

	if (m_iBackIndex > 7)
		m_iBackIndex = 0;

	m_iMaxHp = (_float)m_pKamenInfo->fMaxHp;

	return S_OK;
}

void CBossHpBar::Priority_Update(_float fTimeDelta)
{
}

void CBossHpBar::Update(_float fTimeDelta)
{
	_uint iNumLine = (_uint)m_pKamenInfo->fHp / 10000000;

	m_fValue = (_float)((_uint)m_pKamenInfo->fHp % 10000000) / 10000000.f;
	
	m_iFrontIndex = (_uint)(m_pKamenInfo->fHp / 10000000.f) % 7;
	m_iBackIndex = m_iFrontIndex + 1;

	if (m_iBackIndex > 7)
		m_iBackIndex = 0;

	_uint iHp = m_pKamenInfo->fHp;

	m_HpFontDesc.strWord = to_wstring(iHp) + L"/" + to_wstring(m_iMaxHp);

	m_LineFontDesc.strWord = L"X " + to_wstring(iNumLine);
}

void CBossHpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &m_HpFontDesc);
	m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_LineFontDesc);
}

HRESULT CBossHpBar::Render()
{

	// UI
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	// Background
	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iBackIndex))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	// Render Fill
	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iFrontIndex))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fValue", &m_fValue, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#pragma region TEST_CODE
	FONT_DESC* pFontDesc = &m_LineFontDesc;
	ImGui::InputFloat("X", &pFontDesc->vPositon.x, 1.f, 10.f);
	ImGui::InputFloat("Y", &pFontDesc->vPositon.y, 1.f, 10.f);
	ImGui::InputFloat("Size", &pFontDesc->fScale, 0.01f, 0.1f);
	ImGui::ColorEdit4(
		"Color", (float*)&pFontDesc->vColor,
		ImGuiColorEditFlags_AlphaBar
		| ImGuiColorEditFlags_AlphaPreviewHalf
		| ImGuiColorEditFlags_DisplayRGB
		| ImGuiColorEditFlags_PickerHueWheel);

#pragma endregion

	return S_OK;
}

HRESULT CBossHpBar::Ready_Font()
{
	m_HpFontDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_HpFontDesc.strWord = L"123456789 / 126354987456";
	m_HpFontDesc.vPositon = _float2(595.f, 52.f);
	m_HpFontDesc.fScale = 0.28f;

	m_LineFontDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_LineFontDesc.strWord = L"X 999";
	m_LineFontDesc.vPositon = _float2(845.f, 52.f);
	m_LineFontDesc.fScale = 0.28f;

	return S_OK;
}

HRESULT CBossHpBar::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_BossHpBar"),
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

HRESULT CBossHpBar::Draw()
{
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CBossHpBar* CBossHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossHpBar* pInstance = new CBossHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBossHpBar");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBossHpBar::Clone(void* pArg)
{
	CGameObject* pInstance = new CBossHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBossHpBar");
		return nullptr;
	}
	return pInstance;
}

void CBossHpBar::Free()
{
	__super::Free();
}
