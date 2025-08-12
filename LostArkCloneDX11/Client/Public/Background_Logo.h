#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Engine)
class CUIAnimation;
NS_END

NS_BEGIN(Client)

class CBackground_Logo final : public CUIPanel
{
private:
	CBackground_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackground_Logo(const CBackground_Logo& Prototype);
	virtual ~CBackground_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CUIAnimation*	m_pAnimCom = { nullptr };
	CTexture*		m_pTextureCom_ExitBtn = { nullptr };


private:
	void ExitGame();
	HRESULT			Add_Components();
	HRESULT			Add_Buttons();


public:
	static CBackground_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END