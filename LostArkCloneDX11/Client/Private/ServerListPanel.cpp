#include "pch.h"
#include "ServerListPanel.h"

#include "GameInstance.h"

#include "UIButton.h"
#include "Level_Loading.h"

CServerListPanel::CServerListPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{pDevice, pContext}
{
}

CServerListPanel::CServerListPanel(const CServerListPanel& Prototype)
	: CUIPanel{Prototype}
{
}

HRESULT CServerListPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CServerListPanel::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 100.f;
	Desc.fY = 130.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 680.f;
	Desc.fSizeY = 345.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Add_Buttons()))
		return E_FAIL;

	Add_Fonts();
	

	return S_OK;
}

void CServerListPanel::Priority_Update(_float fTimeDelta)
{
}

void CServerListPanel::Update(_float fTimeDelta)
{
	Update_Position();

}

void CServerListPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	m_pGameInstance->Add_FontDesc(TEXT("Title_Font"), &m_Font_Title);
	m_pGameInstance->Add_FontDesc(TEXT("Title_Font"), &m_Font_SubTitle);
}

HRESULT CServerListPanel::Render()
{
	if (FAILED(__super::Bind_ShaderResource(0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CServerListPanel::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_ServerListBack"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_EmptyButton"),
		TEXT("Com_Texture_Btn"), reinterpret_cast<CComponent**>(&m_pTextureCom_Btn))))
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
HRESULT CServerListPanel::Add_Buttons()
{
	/* Exit Button */
	CUIButton::BUTTON_DESC Desc = {};

	Desc.fRotatePersec = 1.f;
	Desc.fSpeedPersec = 1.f;
	Desc.fX = -100.f;
	Desc.fY = -100.f;
	Desc.fZ = m_fZ;
	Desc.fSizeX = 490.f;
	Desc.fSizeY = 32.f;
	Desc.pParent_TransformCom = m_pTransformCom;
	Desc.pShaderCom = m_pShaderCom;
	Desc.pTextureCom = m_pTextureCom_Btn;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(PROTOTYPE::GAMEOBJECT), TEXT("Prototype_GameObject_Button"),
		ENUM_TO_INT(LEVEL::LOGO), TEXT("Layer_ServerButton"), &Desc);

	Desc.fY = -68.f;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(PROTOTYPE::GAMEOBJECT), TEXT("Prototype_GameObject_Button"),
		ENUM_TO_INT(LEVEL::LOGO), TEXT("Layer_ServerButton"), &Desc);

	if (FAILED(Add_ChildObjects(ENUM_TO_INT(LEVEL::LOGO), TEXT("Layer_ServerButton"))))
		return E_FAIL;

	return S_OK;
}
void CServerListPanel::Add_Fonts()
{
	m_Font_Title.strWord = wstring(L"서버 선택");
	m_Font_Title.vPositon = _float4(593.f, 325.f, 1.f, 1.f);
	m_Font_Title.vColor = _float4(0.85f, 0.85f, 0.85f, 1.f);
	m_Font_Title.fScale = 0.97f;

	m_Font_SubTitle.strWord = wstring(L"서버                         상태                캐릭터");
	m_Font_SubTitle.vPositon = _float4(525.f, 358.f, 1.f, 1.f);
	m_Font_SubTitle.vColor = _float4(0.75f, 0.61f, 0.42f, 1.f);
	m_Font_SubTitle.fScale = 0.78f;
}

CServerListPanel* CServerListPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CServerListPanel* pInstance = new CServerListPanel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CServerListPanel");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CServerListPanel::Clone(void* pArg)
{
	CGameObject* pInstance = new CServerListPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CServerListPanel");
		return nullptr;
	}
	return pInstance;
}

void CServerListPanel::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom_Btn);
}
