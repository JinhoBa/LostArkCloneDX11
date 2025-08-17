#pragma once
#include "Client_Defines.h"

#include "QuickSlot.h"

NS_BEGIN(Client)

class CSkillSlot : public CQuickSlot
{
public:
	typedef struct SkillSlot_Desc : public CQuickSlot::QUICKSLOT_DESC
	{
		_uint iSubSlotID = {};
	}SKILLSLOT_DESC;
private:
	CSkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillSlot(const CSkillSlot& Prototype);
	virtual ~CSkillSlot() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	STANCE		m_eStance = {};
	_uint		m_iSkillID[ENUM_TO_INT(STANCE::IDEN_END)] = {};
	_float		m_fCoolTime = {};

	CTexture*	m_pTextureCom_Skill = { nullptr };


private:
	HRESULT Add_Components();
	

public:
	static CSkillSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
