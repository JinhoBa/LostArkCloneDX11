#include "pch.h"
#include "TopMenu.h"

#include "GameInstance.h"

CTopMenu::CTopMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CTopMenu::CTopMenu(const CTopMenu& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CTopMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTopMenu::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = -134.f;
	Desc.fY = -337.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 1300.f;
	Desc.fSizeY = 43.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Layer(TEXT("Layer_TopMenu"))))
		return E_FAIL;


	return S_OK;
}

void CTopMenu::Priority_Update(_float fTimeDelta)
{
}

void CTopMenu::Update(_float fTimeDelta)
{
	Update_Position();
}

void CTopMenu::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CTopMenu::Render()
{
#pragma region TESTCODE
	ImGui::InputFloat("X", &m_fRX, 1.f, 10.f, "%.0f");
	ImGui::InputFloat("Y", &m_fRY, 1.f, 10.f, "%.0f");
	ImGui::InputFloat("SizeX", &m_fSizeX, 1.f, 10.f, "%.0f");
	ImGui::InputFloat("SizeY", &m_fSizeY, 1.f, 10.f, "%.0f");
#pragma endregion

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

HRESULT CTopMenu::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TopMenu"),
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

HRESULT CTopMenu::Ready_Layer(const _wstring& strLayerTag)
{
	/*UIOBJECT_DESC Desc = {};

	Desc.pParent_TransformCom = m_pTransformCom;

	if (FAILED(Add_ChildObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;*/

	return S_OK;
}

CTopMenu* CTopMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTopMenu* pInstance = new CTopMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CTopMenu");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CTopMenu::Clone(void* pArg)
{
	CGameObject* pInstance = new CTopMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CTopMenu");
		return nullptr;
	}
	return pInstance;
}

void CTopMenu::Free()
{
	__super::Free();
}
