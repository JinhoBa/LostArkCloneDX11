#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
class ENGINE_DLL CUIButton : public CUIObject
{
protected:
	CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIButton(const CUIButton& Prototype);
	virtual ~CUIButton() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	virtual void onButtonClick();
	virtual void onButtonHover();
	void Set_OnClickEvent(function<void()> Event);

protected:
	function<void()> m_OnClickEvent = {};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

public:
	static CUIButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END