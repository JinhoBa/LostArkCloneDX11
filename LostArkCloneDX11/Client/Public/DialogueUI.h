#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Client)

class CDialogueUI final : public CUIPanel
{
private:
	CDialogueUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDialogueUI(CDialogueUI& Prototype);
	virtual ~CDialogueUI() = default;

public:
	_bool Check_Circle();

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	FONT_DESC m_NameFontDesc = {};
	FONT_DESC m_ScriptFontDesc = {};

private:
	HRESULT Ready_Components();
	HRESULT Ready_Font();
	HRESULT Draw_UI();

public:
	static CDialogueUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END