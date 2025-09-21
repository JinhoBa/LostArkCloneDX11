#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CBossHpFrame final : public CUIPanel
{
private:
	CBossHpFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossHpFrame(const CBossHpFrame& Prototype);
	virtual ~CBossHpFrame() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	FONT_DESC				m_NameFontDesc = {};

private:
	HRESULT		Ready_Font();
	HRESULT Add_Components();

public:
	static CBossHpFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END