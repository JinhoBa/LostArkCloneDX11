#include "Client_Defines.h"

#include "HUD.h"

NS_BEGIN(Engine)
class CUIAnimation;
NS_END

NS_BEGIN(Client)

class CIden_Gauge : public CHUD
{
private:
	CIden_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIden_Gauge(const CIden_Gauge& Prototype);
	virtual ~CIden_Gauge() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_uint			m_iTextureIndex = {};

	CTexture*		m_pFireTextureCom = { nullptr };
	CUIAnimation*	m_pAnimationCom = {nullptr};
	_float			m_fGauge = {};
	

private:
	HRESULT Add_Components();

public:
	static CIden_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
