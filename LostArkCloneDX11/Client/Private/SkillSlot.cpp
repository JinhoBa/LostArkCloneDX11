#include "pch.h"
#include "SkillSlot.h"

#include "GameInstance.h"
#include "GameManager.h"

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

	SKILL_INFO* pInfo = { nullptr };

	pInfo = m_pGameManager->Get_SkillInfo_Prt(m_iSkillID[0]);
	if (nullptr != pInfo)
		m_fMaxCoolTime[0] = pInfo->fCoolTime;
	else
		m_fMaxCoolTime[0] = 0.f;

	pInfo = m_pGameManager->Get_SkillInfo_Prt(m_iSkillID[1]);
	if (nullptr != pInfo)
		m_fMaxCoolTime[1] = pInfo->fCoolTime;
	else
		m_fMaxCoolTime[1] = 0.f;


	m_eStance = STANCE::FLURRY;
	m_iPassIndex = 0;

	return S_OK;
}

void CSkillSlot::Priority_Update(_float fTimeDelta)
{
}

void CSkillSlot::Update(_float fTimeDelta)
{
	if (m_eStance != m_pPlayerInfo->eStance)
	{
		m_eStance = m_pPlayerInfo->eStance;
	}

	m_fCoolTime = m_pGameManager->Check_CoolTime(m_iSkillID[ENUM_TO_INT(m_eStance)]);

	if (0.f == m_fCoolTime)
		m_iPassIndex = 0;
	else
		m_iPassIndex = 5;

}

void CSkillSlot::Late_Update(_float fTimeDelta)
{
	if(0.f != m_fCoolTime)
	{
		size_t NumStr = {};
		if (9.f <= m_fCoolTime)
		{
			m_Font_CoolTime.strWord = to_wstring(m_fCoolTime + 1.f).substr(0, 2) + L"s";
		}
		else
		{
			m_Font_CoolTime.strWord = L" " + to_wstring(m_fCoolTime + 1.f).substr(0, 1) + L"s";
		}
		
		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_Font_CoolTime);
	}

	__super::Late_Update(fTimeDelta);
}

HRESULT CSkillSlot::Render()
{


	if (FAILED(__super::Bind_Resource()))
		return E_FAIL;

	if (FAILED(Render_SlotBack()))
		return E_FAIL;

	if(99 != m_iSkillID[ENUM_TO_INT(m_eStance)])
	{
		if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom_Skill->Get_SRV(m_iSkillID[ENUM_TO_INT(m_eStance)]))))
			return E_FAIL;

		if(0.f != m_fCoolTime)
		{
			_float fCooltime = (m_fMaxCoolTime[ENUM_TO_INT(m_eStance)] - m_fCoolTime) / m_fMaxCoolTime[ENUM_TO_INT(m_eStance)];

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fValue", &fCooltime, sizeof(_float))))
				return E_FAIL;
		}

		if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	if (FAILED(Render_SlotFront()))
		return E_FAIL;

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
