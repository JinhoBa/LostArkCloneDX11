#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Client)

class CWallpaper final : public CUIPanel
{
private:
	CWallpaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWallpaper(const CWallpaper& Prototype);
	virtual ~CWallpaper() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT			Add_Components();

public:
	static CWallpaper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END