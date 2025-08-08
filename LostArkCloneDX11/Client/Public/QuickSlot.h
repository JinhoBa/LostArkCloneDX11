#pragma once
#include "Client_Defines.h"

#include "UIPanel.h"

NS_BEGIN(Client)

class CQuickSlot : public CUIPanel
{
public:
	typedef struct QuickSlot_Desc : public CUIObject::UIOBJECT_DESC
	{
		_byte byKey = {};
	}QUICKSLOT_DESC;
protected:
	CQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuickSlot(const CQuickSlot& Prototype);
	virtual ~CQuickSlot() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	_byte			m_byKey = {};
	_uint			m_iSlotID = {};
	_uint			m_iTextureIndex = {};
	CTexture*		m_pFrameTextureCom = { nullptr };

protected:
	HRESULT Bind_Resource();
	HRESULT Draw();

private:
	HRESULT Add_Components();

public:
	static CQuickSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
