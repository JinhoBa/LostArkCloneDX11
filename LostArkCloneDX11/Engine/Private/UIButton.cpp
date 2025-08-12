#include "UIButton.h"

#include "GameInstance.h"

CUIButton::CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{pDevice, pContext}
{
}

CUIButton::CUIButton(const CUIButton& Prototype)
	:CUIObject{Prototype}
{
}

HRESULT CUIButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIButton::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

	m_pTextureCom = pDesc->pTextureCom;
	m_pShaderCom = pDesc->pShaderCom;
	m_OnHoverEvent = pDesc->OnHoverEvent;
	m_OnClickEvent = pDesc->OnClickEvent;

	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pShaderCom);

	m_pVIBufferCom = CVIBuffer_Rect::Create(m_pDevice, m_pContext);

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

	Safe_AddRef(m_pVIBufferCom);

	if (FAILED(m_pVIBufferCom->Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_rcButton = {
	(_long)(m_fX - m_fSizeX * 0.5f),
	(_long)(m_fY - m_fSizeY * 0.5f),
	(_long)(m_fX + m_fSizeX * 0.5f),
	(_long)(m_fY + m_fSizeY * 0.5f),
	};

	return S_OK;
}

void CUIButton::Priority_Update(_float fTimeDelta)
{
}

void CUIButton::Update(_float fTimeDelta)
{
#pragma region TESTCODE
	Update_Position();
#pragma endregion

	if (isInRect())
	{
		m_iTextIndex = 1;
		if(nullptr != m_OnHoverEvent)
			m_OnHoverEvent();

		if (m_pGameInstance->Get_DIMouseDown(MOUSEKEYSTATE::LBUTTON))
			if (nullptr != m_OnClickEvent)
				m_OnClickEvent();
	}
	else
		m_iTextIndex = 0;
}

void CUIButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIButton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iTextIndex))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CUIButton::Set_OnHoverEvent(function<void()> Event)
{
	m_OnHoverEvent = Event;
}

void CUIButton::Set_OnClickEvent(function<void()> Event)
{
	m_OnClickEvent = Event;
}

_bool CUIButton::isInRect()
{
	return PtInRect(&m_rcButton, m_pGameInstance->Get_MousePoint());
}

_bool CUIButton::isInCircle()
{

	return true;
}

CUIButton* CUIButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIButton* pInstance = new CUIButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CUIButton");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIButton::Clone(void* pArg)
{
	CGameObject* pInstance = new CUIButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CUIButton");
		return nullptr;
	}

	return pInstance;
}

void CUIButton::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
