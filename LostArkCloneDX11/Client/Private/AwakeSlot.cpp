#include "pch.h"
#include "AwakeSlot.h"

#include "GameInstance.h"
#include "GameManager.h"

CAwakeSlot::CAwakeSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CQuickSlot{ pDevice, pContext }
{
}

CAwakeSlot::CAwakeSlot(const CAwakeSlot& Prototype)
	: CQuickSlot{ Prototype },
	m_iPassIndex{ 0 }, m_fCoolTime{ 0.f }, m_iSkillID{ 14 }
{
}

HRESULT CAwakeSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAwakeSlot::Initialize(void* pArg)
{
	m_byKey = DIK_V;

	QUICKSLOT_DESC Desc= {};
	Desc.pKey = L"V";
	Desc.byKey = DIK_V;
	Desc.fX = -330.f;
	Desc.fY = 25.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (nullptr == pArg)
		return E_FAIL;

	SKILL_INFO* pInfo = { nullptr };

	pInfo = m_pGameManager->Get_SkillInfo_Prt(m_iSkillID);
	if (nullptr != pInfo)
		m_fMaxCoolTime = pInfo->fCoolTime;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CAwakeSlot::Priority_Update(_float fTimeDelta)
{
}

void CAwakeSlot::Update(_float fTimeDelta)
{
	Update_Position();

	m_fCoolTime = m_pGameManager->Check_CoolTime(m_iSkillID);

	if (0.f != m_fCoolTime)
		m_iPassIndex = 5;
	else
		m_iPassIndex = 0;
}

void CAwakeSlot::Late_Update(_float fTimeDelta)
{
	if (0.f != m_fCoolTime)
	{
		_uint iMinute = (_uint)m_fCoolTime / 60;
		if (1 > iMinute)
		{
			m_Font_CoolTime.strWord = L" " + to_wstring(m_fCoolTime + 1.f).substr(0, 1) + L"m";
		}
		else
		{
			if (9.f <= m_fCoolTime)
			{
				m_Font_CoolTime.strWord = to_wstring(m_fCoolTime + 1.f).substr(0, 2) + L"s";
			}
			else
			{
				m_Font_CoolTime.strWord = L" " + to_wstring(m_fCoolTime + 1.f).substr(0, 1) + L"s";
			}
		}

		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_Font_CoolTime);
	}

	__super::Late_Update(fTimeDelta);
}

HRESULT CAwakeSlot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pAwakeTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (0.f != m_fCoolTime)
	{
		_float4 vCooltime = _float4((m_fMaxCoolTime - m_fCoolTime) / m_fMaxCoolTime, 0.f, 0.f, 0.f);

		if (FAILED(m_pShaderCom->Bind_Value("g_Vecotr", &vCooltime)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CAwakeSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_EpicSkill"),
		TEXT("Com_AwakeTexture"), reinterpret_cast<CComponent**>(&m_pAwakeTextureCom))))
		return E_FAIL;

	return S_OK;
}

CAwakeSlot* CAwakeSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAwakeSlot* pInstance = new CAwakeSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAwakeSlot");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CAwakeSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CAwakeSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CAwakeSlot");
		return nullptr;
	}
	return pInstance;
}

void CAwakeSlot::Free()
{
	__super::Free();

	Safe_Release(m_pAwakeTextureCom);
}
