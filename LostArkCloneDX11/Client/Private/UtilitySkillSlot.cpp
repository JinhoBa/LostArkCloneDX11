#include "pch.h"
#include "UtilitySkillSlot.h"

#include "GameInstance.h"
#include "GameManager.h"

CUtilitySkillSlot::CUtilitySkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHUD{ pDevice, pContext }, m_pGameManager{ CGameManager::GetInstance() }
{
	Safe_AddRef(m_pGameManager);
}

CUtilitySkillSlot::CUtilitySkillSlot(const CUtilitySkillSlot& Prototype)
	: CHUD{ Prototype }, m_pGameManager{ Prototype.m_pGameManager }
{
	Safe_AddRef(m_pGameManager);
}

HRESULT CUtilitySkillSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUtilitySkillSlot::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	UTILITYSLOT_DESC* pDesc = static_cast<UTILITYSLOT_DESC*>(pArg);

	m_iSlotID = pDesc->iSlotID;

	m_iTextureIndex[0] = pDesc->iTextureIndex;
	m_iTextureIndex[1] = pDesc->iSubTextureIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	m_fMaxCoolTime = m_pGameManager->Get_SkillInfo_Prt(m_iSlotID)->fCoolTime;

	return S_OK;
}

void CUtilitySkillSlot::Priority_Update(_float fTimeDelta)
{

}

void CUtilitySkillSlot::Update(_float fTimeDelta)
{
	m_fCoolTime = m_pGameManager->Check_CoolTime(m_iSlotID);
}

void CUtilitySkillSlot::Late_Update(_float fTimeDelta)
{
	if (0.f != m_fCoolTime)
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

HRESULT CUtilitySkillSlot::Render()
{
	Update_Position();

	if (m_fCoolTime <= 0.f)
		return S_OK;

	if (FAILED(Bind_Resource()))
		return E_FAIL;

	if (FAILED(Render_SlotBack()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pSkillTextureCom->Get_SRV(m_iTextureIndex[ENUM_TO_INT(m_pPlayerInfo->eStance)]))))
		return E_FAIL;

	if (0.f != m_fCoolTime)
	{
		_float vCooltime = (m_fMaxCoolTime - m_fCoolTime) / m_fMaxCoolTime;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fValue", &vCooltime, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUtilitySkillSlot::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		TEXT("Com_FrameTexture"), reinterpret_cast<CComponent**>(&m_pFrameTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_UtilitySkill"),
		TEXT("Com_SkillTexture"), reinterpret_cast<CComponent**>(&m_pSkillTextureCom))))
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

HRESULT CUtilitySkillSlot::Bind_Resource()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUtilitySkillSlot::Draw()
{
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUtilitySkillSlot::Render_SlotBack()
{
	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pFrameTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUtilitySkillSlot::Ready_Font()
{
	m_Font_CoolTime.strWord = L"";
	m_Font_CoolTime.vPositon = _float2(m_fX - 15.f, m_fY - 9.f);
	m_Font_CoolTime.vColor = _float4(0.9f, 0.9f, 0.9f, 1.f);
	m_Font_CoolTime.fScale = 0.4f;

	return S_OK;
}

CUtilitySkillSlot* CUtilitySkillSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUtilitySkillSlot* pInstance = new CUtilitySkillSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CUtilitySkillSlot");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUtilitySkillSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CUtilitySkillSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CUIPanel");
		return nullptr;
	}

	return pInstance;
}

void CUtilitySkillSlot::Free()
{
	__super::Free();

	Safe_Release(m_pFrameTextureCom);
	Safe_Release(m_pSkillTextureCom);
	Safe_Release(m_pGameManager);
}
