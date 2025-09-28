#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CBossHpBar : public CUIBar
{
private:
	CBossHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossHpBar(const CBossHpBar& Prototype);
	virtual ~CBossHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	const ENEMY_INFO*		m_pKamenInfo = { nullptr };
	_uint					m_iFrontIndex = {};
	_uint					m_iBackIndex = {};

	_uint					m_iMaxHp = {};
	_float					m_fHpPerBar = {};

	FONT_DESC				m_HpFontDesc = {};
	FONT_DESC				m_LineFontDesc = {};

private:
	HRESULT		Ready_Font();
	HRESULT		Add_Components();
	HRESULT		Draw();

public:
	static CBossHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END