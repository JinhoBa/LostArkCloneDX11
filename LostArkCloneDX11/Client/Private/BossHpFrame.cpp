#include "pch.h"
#include "BossHpFrame.h"

#include "GameInstance.h"

CBossHpFrame::CBossHpFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBossHpFrame::CBossHpFrame(const CBossHpFrame& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CBossHpFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossHpFrame::Initialize(void* pArg)
{
	UIOBJECT_DESC* pParent_Desc = static_cast<UIOBJECT_DESC*>(pArg);

	UIOBJECT_DESC Desc = {};

	Desc.fX = -30.f;
	Desc.fY = -2.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 790.f;
	Desc.fSizeY = 90.f;
	Desc.pParent_TransformCom = pParent_Desc->pParent_TransformCom;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;


	return S_OK;
}

void CBossHpFrame::Priority_Update(_float fTimeDelta)
{
}

void CBossHpFrame::Update(_float fTimeDelta)
{
}

void CBossHpFrame::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_NameFontDesc);

	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHpFrame::Render()
{


	// UI
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBossHpFrame::Ready_Font()
{
	m_NameFontDesc.vColor = _float4(0.6f, 0.f, 0.f, 1.f);
	m_NameFontDesc.strWord = L"ºûÀ» ²¨Æ®¸®´Â ÀÚ, Ä«¸à";
	m_NameFontDesc.vPositon = _float2(590.f, 26.f);
	m_NameFontDesc.fScale = 0.31f;

	return S_OK;
}

HRESULT CBossHpFrame::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_BossHpBarFrame"),
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

CBossHpFrame* CBossHpFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossHpFrame* pInstance = new CBossHpFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBossHpFrame");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBossHpFrame::Clone(void* pArg)
{
	CGameObject* pInstance = new CBossHpFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBossHpFrame");
		return nullptr;
	}
	return pInstance;
}

void CBossHpFrame::Free()
{
	__super::Free();
}
