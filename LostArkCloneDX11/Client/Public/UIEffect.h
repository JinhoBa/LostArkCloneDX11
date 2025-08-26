#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Client)

class CUIEffect abstract : public CUIPanel
{
public:
	typedef struct UIEffect_Desc : CUIObject::UIOBJECT_DESC
	{
		_float fDuration{};
	}UIEFFECT_DESC;
private:
	CUIEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIEffect(const CUIEffect& Prototype);
	virtual ~CUIEffect() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_float	m_fTimeAcc = {};
	_float	m_fDuration = {};

private:
	HRESULT Add_Components();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END
