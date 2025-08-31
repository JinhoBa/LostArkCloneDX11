#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CHUD  : public CUIPanel
{
protected:
	CHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHUD(const CHUD& Prototype);
	virtual ~CHUD() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	PLAYER_INFO* m_pPlayerInfo = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END
