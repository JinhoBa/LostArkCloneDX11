#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CWeaponUI : public CUIPanel
{
	enum class STATE { FLURRY, FOCUS, TO_FLURRY, TO_FOCUS, END };
private:
	CWeaponUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponUI(const CWeaponUI& Prototype);
	virtual ~CWeaponUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	STATE			m_ePreState = {};
	STATE			m_eCurState = {};

	_uint			m_iTextureIndex = {};
	_uint			m_iNumShake = {};

	_float			m_fTimeAcc = {};

	_float			m_fShakeSpeed = {};
	_float			m_fTurnTime = {};

	_float          m_fPreY = {};

private:
	HRESULT Add_Components();
	void	Change_State();
	void	Shake(_float fTimeDelta);
	void    UpAndDown(_float fTimeDelta);

public:
	static CWeaponUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
