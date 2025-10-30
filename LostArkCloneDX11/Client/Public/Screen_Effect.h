#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Client)

class CScreen_Effect final : public CUIPanel
{
private:
	CScreen_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen_Effect(CScreen_Effect& Prototype);
	virtual ~CScreen_Effect() = default;

public:
	void			Trigger_FadeIn(_float fDuration);
	void			Trigger_FadeOut(_float fDuration);

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_bool			m_isFadeIn = {};
	_bool			m_isFadeOut = {};

	_float			m_fDuration = {};
	_float			m_fTimeAcc = {};

	_float4			m_vScreenColor = {};

private:
	HRESULT Ready_Components();
	void FadeIn(_float fTimeDelta);
	void FadeOut(_float fTimeDelta);

public:
	static CScreen_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END