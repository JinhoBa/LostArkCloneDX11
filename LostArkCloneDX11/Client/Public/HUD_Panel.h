#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CHUD_Panel : public CUIPanel
{
private:
	CHUD_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHUD_Panel(const CHUD_Panel& Prototype);
	virtual ~CHUD_Panel() = default;

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
	HRESULT Add_QuickSlots(const _wstring& strLayerTag);

public:
	static CHUD_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END