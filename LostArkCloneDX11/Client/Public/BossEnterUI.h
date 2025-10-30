#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CBossEnterUI final : public CUIPanel
{
private:
	CBossEnterUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossEnterUI(const CBossEnterUI& Prototype);
	virtual ~CBossEnterUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_bool					m_isVisible = {};

	_uint					m_iPreCount = {};
	_float					m_fTimeAcc = {};

	FONT_DESC				m_MessageFontDesc = {};
	FONT_DESC				m_CountFontDesc = {};
	
	class CEffect_BossEnter* m_pBossEnterEffect = { nullptr };

private:
	HRESULT		Ready_Font();
	HRESULT		Add_Components();
	HRESULT		Enter_Boss();

public:
	static CBossEnterUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END