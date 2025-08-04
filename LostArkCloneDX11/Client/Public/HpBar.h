#pragma once
#include "Client_Defines.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CHpBar : public CUIBar
{
private:
	CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar(const CHpBar& Prototype);
	virtual ~CHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT		Add_Components();

public:
	static CHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
