#pragma once
#include "Client_Defines.h"

#include "QuickSlot.h"

NS_BEGIN(Client)

class CAwakeSlot : public CQuickSlot
{
private:
	CAwakeSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAwakeSlot(const CAwakeSlot& Prototype);
	virtual ~CAwakeSlot() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_uint		m_iSkillID = {};
	_uint		m_iPassIndex = {};

	_float		m_fMaxCoolTime = {};
	_float		m_fCoolTime = {};

	CTexture*	m_pAwakeTextureCom = { nullptr };

private:
	HRESULT Add_Components();

public:
	static CAwakeSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
