#include "pch.h"
#include "HUD.h"

#include "GameInstance.h"

#include "SkillSlot.h"

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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Iden_Gauge"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(Add_QuickSlots(strLayerTag)))
		return E_FAIL;

	if (FAILED(Add_ChildObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHUD::Add_QuickSlots(const _wstring& strLayerTag)
{
	_uint iKeyIndex = {};
	_byte SlotKeys[] = {
		DIK_Q, DIK_W, DIK_E, DIK_R,
		DIK_A, DIK_S, DIK_D, DIK_F,
		DIK_7, DIK_6, DIK_5, DIK_F1,
		DIK_4, DIK_3, DIK_2, DIK_1,
	};
	const _tchar* pKeys[] = {
		L"Q", L"W", L"E", L"R",
		L"A", L"S", L"D", L"F",
		L"7", L"6", L"5", L"F1",
		L"4", L"3", L"2", L"1",
	};
	_uint SlotIDs[] = {
		0, 1, 2, 3,
		4, 5, 6, 7
	};
	_uint SubSlotIDs[] = {
		8, 9, 99, 10,
		99, 11, 12, 13
	};

	CSkillSlot::SKILLSLOT_DESC QuickSlot_Desc = {};
	QuickSlot_Desc.fSizeX = 40.f;
	QuickSlot_Desc.fSizeY = 40.f;
	QuickSlot_Desc.fZ = 0.5f;
	QuickSlot_Desc.pParent_TransformCom = m_pTransformCom;
	QuickSlot_Desc.fY = 9.f;
	
	_float offset = 215.f;
	for (size_t i = 0; i < 4; i++)
	{
		QuickSlot_Desc.fX = i * QuickSlot_Desc.fSizeX - offset;
		QuickSlot_Desc.pKey = pKeys[iKeyIndex];
		QuickSlot_Desc.iSlotID = SlotIDs[iKeyIndex];
		QuickSlot_Desc.iSubSlotID = SubSlotIDs[iKeyIndex];
		QuickSlot_Desc.byKey = SlotKeys[iKeyIndex++];
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkillSlot"),
			ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &QuickSlot_Desc)))
			return E_FAIL;
	}

	QuickSlot_Desc.fY = 49.f;
	for (size_t i = 0; i < 4; i++)
	{
		QuickSlot_Desc.fX = i * QuickSlot_Desc.fSizeX + QuickSlot_Desc.fSizeX* 0.5f - offset;
		QuickSlot_Desc.pKey = pKeys[iKeyIndex];
		QuickSlot_Desc.iSlotID = SlotIDs[iKeyIndex];
		QuickSlot_Desc.iSubSlotID = SubSlotIDs[iKeyIndex];
		QuickSlot_Desc.byKey = SlotKeys[iKeyIndex++];
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkillSlot"),
			ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &QuickSlot_Desc)))
			return E_FAIL;
	}
	QuickSlot_Desc.fY = 9.f;
	for (size_t i = 0; i < 4; i++)
	{
		QuickSlot_Desc.fX = i * QuickSlot_Desc.fSizeX * -1.f+ offset;
		QuickSlot_Desc.pKey = pKeys[iKeyIndex];
		QuickSlot_Desc.byKey = SlotKeys[iKeyIndex++];
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ActiveSlot"),
			ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &QuickSlot_Desc)))
			return E_FAIL;
	}

	QuickSlot_Desc.fY = 49.f;
	for (size_t i = 0; i < 4; i++)
	{
		QuickSlot_Desc.fX = i * QuickSlot_Desc.fSizeX * -1.f + QuickSlot_Desc.fSizeX * -0.5f + offset;
		QuickSlot_Desc.pKey = pKeys[iKeyIndex];
		QuickSlot_Desc.byKey = SlotKeys[iKeyIndex++];
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ActiveSlot"),
			ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &QuickSlot_Desc)))
			return E_FAIL;
	}

	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AwakeSlot"),
		ENUM_TO_INT(LEVEL::GAMEPLAY), strLayerTag, &QuickSlot_Desc)))
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
