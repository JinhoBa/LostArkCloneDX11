#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CTopMenu : public CUIPanel
{
private:
	CTopMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTopMenu(const CTopMenu& Prototype);
	virtual ~CTopMenu() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Add_Components();
	HRESULT Ready_Layer(const _wstring& strLayerTag);

public:
	static CTopMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END