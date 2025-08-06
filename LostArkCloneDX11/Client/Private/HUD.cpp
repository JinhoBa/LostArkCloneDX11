#include "pch.h"
#include "HUD.h"

#include "GameInstance.h"

CHUD::CHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CHUD::CHUD(const CHUD& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CHUD::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHUD::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 285.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 500.f;
	Desc.fSizeY = 120.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Layer(TEXT("Layer_HUD"))))
		return E_FAIL;


	return S_OK;
}

void CHUD::Priority_Update(_float fTimeDelta)
{
}

void CHUD::Update(_float fTimeDelta)
{
	Update_Position();
}

void CHUD::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CHUD::Render()
{
	//if (FAILED(__super::Bind_ShaderResource(0)))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(0)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Bind_Resources()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHUD::Add_Components()
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

HRESULT CHUD::Ready_Layer(const _wstring& strLayerTag)
{
	UIOBJECT_DESC Desc = {};

	Desc.pParent_TransformCom = m_pTransformCom;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hpbar"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Mpbar"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WeaponUI"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Identity"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	

	if (FAILED(Add_ChildObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CHUD* CHUD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHUD* pInstance = new CHUD(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CHUD");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHUD::Clone(void* pArg)
{
	CGameObject* pInstance = new CHUD(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CHUD");
		return nullptr;
	}
	return pInstance;
}

void CHUD::Free()
{
	__super::Free();
}
