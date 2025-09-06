#pragma once
#include "Client_Defines.h"

#include "HUD.h"

NS_BEGIN(Client)

class CUtilitySkillUI : public CHUD
{
protected:
	CUtilitySkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUtilitySkillUI(const CUtilitySkillUI& Prototype);
	virtual ~CUtilitySkillUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_uint								m_iNumSkill = {};
	vector<class CUtilitySkillSlot*>	m_UtilitySkills;

private:
	HRESULT Add_Slots();

public:
	static CUtilitySkillUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
