#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)

class CCanvars : public CUIPanel
{
private:
	CCanvars(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCanvars(CCanvars& Prototype);

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_pTextureCom = { nullptr };

private:
	HRESULT Add_Components();

public:
	static CCanvars* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END