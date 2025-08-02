#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CLogoPanel : public CUIPanel
{
private:
	CLogoPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoPanel(const CLogoPanel& Prototype);
	virtual ~CLogoPanel() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Add_Components();
public:
	static CLogoPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
