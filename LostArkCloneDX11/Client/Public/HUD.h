#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CHUD : public CUIPanel
{
private:
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

private:
	HRESULT Add_Components();

public:
	static CHUD* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END