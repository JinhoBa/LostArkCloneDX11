#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Engine)
class CUIAnimation;
NS_END

NS_BEGIN(Client)

class CIdentity : public CUIPanel
{
	enum class STATE { FLURRY, FOCUS, TO_FLURRY, TO_FOCUS, END};
private:
	CIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIdentity(const CIdentity& Prototype);
	virtual ~CIdentity() = default;

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

private:
	HRESULT Add_Components();
	void	Change_State();

public:
	static CIdentity* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
