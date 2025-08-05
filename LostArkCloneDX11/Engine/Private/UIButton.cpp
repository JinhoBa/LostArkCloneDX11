#include "UIButton.h"

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

	return S_OK;
}

void CUIButton::Priority_Update(_float fTimeDelta)
{
}

void CUIButton::Update(_float fTimeDelta)
{

}

void CUIButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIButton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CUIButton::onButtonClick()
{
	m_OnClickEvent;
}

void CUIButton::onButtonHover()
{
}

void CUIButton::Set_OnClickEvent(function<void()> Event)
{
	m_OnClickEvent = Event;
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
}
