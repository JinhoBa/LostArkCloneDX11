#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CServerListPanel : public CUIPanel
{
private:
	CServerListPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CServerListPanel(const CServerListPanel& Prototype);
	virtual ~CServerListPanel() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_pTextureCom_Btn = { nullptr };

	FONT_DESC		m_Font_Title = {};
	FONT_DESC		m_Font_SubTitle = {};

private:
	HRESULT Add_Components();
	HRESULT Add_Buttons();
	void	Add_Fonts();

public:
	static CServerListPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END