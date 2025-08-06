#pragma once
#include "Client_Defines.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CExpBar : public CUIBar
{
private:
	CExpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CExpBar(const CExpBar& Prototype);
	virtual ~CExpBar() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT		Add_Components();
	HRESULT		Draw();

public:
	static CExpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END