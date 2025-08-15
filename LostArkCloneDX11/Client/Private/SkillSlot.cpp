#include "pch.h"
#include "SkillSlot.h"

#include "GameInstance.h"

CSkillSlot::CSkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CQuickSlot{ pDevice, pContext }
{
}

CSkillSlot::CSkillSlot(const CSkillSlot& Prototype)
	: CQuickSlot{ Prototype }
{
}

HRESULT CSkillSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillSlot::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILLSLOT_DESC* pDesc = static_cast<SKILLSLOT_DESC*>(pArg);

	m_iSkillID[0] = pDesc->iSlotID;
	m_iSkillID[1] = pDesc->iSubSlotID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eStance = STANCE::FLURRY;

	return S_OK;
}

void CSkillSlot::Priority_Update(_float fTimeDelta)
{
}

void CSkillSlot::Update(_float fTimeDelta)
{
}

void CSkillSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSkillSlot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if(99 != m_iSkillID[ENUM_TO_INT(m_eStance)])
	{
		if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom_Skill->Get_SRV(m_iSkillID[ENUM_TO_INT(m_eStance)]))))
			return E_FAIL;

		if (FAILED(Draw()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkillSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill"),
		TEXT("Com_Texture_Skill"), reinterpret_cast<CComponent**>(&m_pTextureCom_Skill))))
		return E_FAIL;

	return S_OK;
}

CSkillSlot* CSkillSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillSlot* pInstance = new CSkillSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CSkillSlot");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CSkillSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CSkillSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CSkillSlot");
		return nullptr;
	}
	return pInstance;
}

void CSkillSlot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom_Skill);
}
