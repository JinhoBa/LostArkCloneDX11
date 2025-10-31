#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CHoldingSkillUI : public CUIBar
{
private:
	CHoldingSkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHoldingSkillUI(const CHoldingSkillUI& Prototype);
	virtual ~CHoldingSkillUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	class CGameManager*			m_pGameManager = { nullptr };
	_bool						m_isActive = {};
	_float						 m_fTimeAcc = {};
	FONT_DESC					m_SkillName_Font = {};
	FONT_DESC					m_ChargeTime_Font = {};

	const CHARGE_SKILL_DESC*	m_pChargeSkill_Desc = {};


private:
	HRESULT		Add_Components();
	HRESULT     Ready_Font();

public:
	static CHoldingSkillUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
