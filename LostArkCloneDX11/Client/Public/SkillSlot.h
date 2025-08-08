#pragma once
#include "Client_Defines.h"

#include "QuickSlot.h"

NS_BEGIN(Client)

class CSkillSlot : public CQuickSlot
{
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
	_uint		m_iSkillID[IDENTITY::IDEN_END] = {};
	_float		m_fCoolTime = {};

private:
	HRESULT Add_Components();

public:
	static CSkillSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
