#pragma once
#include "Client_Defines.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CMpBar : public CUIBar
{
private:
	CMpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMpBar(const CMpBar& Prototype);
	virtual ~CMpBar() = default;

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
	static CMpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
