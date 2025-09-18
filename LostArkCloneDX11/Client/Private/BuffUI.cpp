#include "pch.h"
#include "BuffUI.h"

#include "GameInstance.h"
#include "Player.h"
#include "Buff.h"

CBuffUI::CBuffUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHUD{ pDevice, pContext }
{
}

CBuffUI::CBuffUI(const CBuffUI& Prototype)
	: CHUD(Prototype)
{
}

HRESULT CBuffUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuffUI::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	UIOBJECT_DESC Desc = {};

	Desc.fX = -105.f;
	Desc.fY = -48.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 25.f;
	Desc.fSizeY = 25.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	m_DefaultFontPosition.x = 523.f;
	m_DefaultFontPosition.y = 607.f;

	m_fDefaultSize = 25.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back());


	return S_OK;
}

void CBuffUI::Priority_Update(_float fTimeDelta)
{
}

void CBuffUI::Update(_float fTimeDelta)
{
	m_BuffIDs.clear();
	m_BuffCoolTimes.clear();

	_uint iIndex = {};

	for(auto& pBuff : m_pPlayer->Get_BuffList())
	{
		m_BuffIDs.push_back(pBuff->Get_BuffID());
		_float fTime = pBuff->Get_CoolTime();

		if (999.f > fTime)
		{
			size_t NumStr = {};
			if (9.f <= fTime)
			{
				m_CoolTimeFont.strWord = to_wstring(fTime + 1.f).substr(0, 2) + L"ÃÊ";
			}
			else
			{
				m_CoolTimeFont.strWord = L" " + to_wstring(fTime + 1.f).substr(0, 1) + L"ÃÊ";
			}
			m_CoolTimeFont.vPositon = _float2(m_DefaultFontPosition.x - (_float)iIndex * 27.f, m_DefaultFontPosition.y);
			m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_CoolTimeFont);
		}
		++iIndex;
	}

	m_iNumBuff = (_uint)m_BuffIDs.size();
}

void CBuffUI::Late_Update(_float fTimeDelta)
{
	

	__super::Late_Update(fTimeDelta);
}

HRESULT CBuffUI::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fX = m_fRX;
	_uint iIndex = {};

	for (auto iID : m_BuffIDs)
	{
		m_fRX -= iIndex * 27.f;
		++iIndex;

		__super::Update_Position();

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(iID))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		m_fSizeX = m_fSizeY = m_fDefaultSize + 2.f;

		__super::Update_Position();

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pFrameTextureCom->Get_SRV(0))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		m_fSizeX = m_fSizeY = m_fDefaultSize;
	}
	
	m_fRX = fX;

	return S_OK;
}

HRESULT CBuffUI::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Buff"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_BuffFrame"),
		TEXT("Com_FrameTexture"), reinterpret_cast<CComponent**>(&m_pFrameTextureCom))))
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

HRESULT CBuffUI::Ready_Font()
{
	m_CoolTimeFont.vColor = _float4(0.5f, 0.8f, 0.5f, 1.f);
	m_CoolTimeFont.strWord = L"";
	m_CoolTimeFont.vPositon = _float2(m_DefaultFontPosition.x, m_DefaultFontPosition.y);
	m_CoolTimeFont.fScale = 0.25f;

	return S_OK;
}

CBuffUI* CBuffUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuffUI* pInstance = new CBuffUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBuffUI");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBuffUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CBuffUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBuffUI");
		return nullptr;
	}
	return pInstance;
}

void CBuffUI::Free()
{
	__super::Free();
}
