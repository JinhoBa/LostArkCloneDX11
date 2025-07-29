#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUIPanel : public CUIObject
{
protected:
	CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPanel(const CUIPanel& Prototype);
	virtual ~CUIPanel() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	static CUIPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END