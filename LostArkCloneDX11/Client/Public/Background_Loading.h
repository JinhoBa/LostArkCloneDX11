#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Engine)
class CFont;
NS_END

NS_BEGIN(Client)

class CBackground_Loading final : public CUIPanel
{
private:
	CBackground_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackground_Loading(const CBackground_Loading& Prototype);
	virtual ~CBackground_Loading() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	FONT_DESC				m_Font_Tip = {};
	FONT_DESC				m_Font_TipDetail = {};
	FONT_DESC				m_Font_Mococo = {};

private:
	HRESULT			Add_Components();
	void			Reay_FontDesc();

public:
	static CBackground_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END