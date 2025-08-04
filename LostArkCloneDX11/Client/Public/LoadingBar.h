#pragma once
#include "Client_Defines.h"
#include "UIBar.h"

NS_BEGIN(Client)

class CLoadingBar : public CUIBar
{
private:
	CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingBar(const CLoadingBar& Prototype);
	virtual ~CLoadingBar() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	class CLevel_Loading*	m_pLoading = { nullptr };

private:
	HRESULT		Add_Components();

public:
	static CLoadingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END