#include "pch.h"
#include "BossUI.h"

#include "GameInstance.h"

CBossUI::CBossUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBossUI::CBossUI(const CBossUI& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CBossUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossUI::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = -300.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 660.f;
	Desc.fSizeY = 110.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Layer(TEXT("Layer_BossUI"))))
		return E_FAIL;


	return S_OK;
}

void CBossUI::Priority_Update(_float fTimeDelta)
{
}

void CBossUI::Update(_float fTimeDelta)
{
}

void CBossUI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CBossUI::Render()
{
	/*ÇÏÀ§ °èÃþ¸¸ ·»´õ*/
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossUI::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Wallpaper"),
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

HRESULT CBossUI::Ready_Layer(const _wstring& strLayerTag)
{
	UIOBJECT_DESC Desc = {};

	Desc.pParent_TransformCom = m_pTransformCom;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossHpBar"),
		ENUM_TO_INT(LEVEL::BOSS), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossHpFrame"),
		ENUM_TO_INT(LEVEL::BOSS), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(Add_ChildObjects(ENUM_TO_INT(LEVEL::BOSS), strLayerTag)))
		return E_FAIL;

	return S_OK;
}


CBossUI* CBossUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossUI* pInstance = new CBossUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBossUI");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBossUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CBossUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBossUI");
		return nullptr;
	}
	return pInstance;
}

void CBossUI::Free()
{
	__super::Free();
}
