#pragma once
#include "Client_Defines.h"

#include "QuickSlot.h"

NS_BEGIN(Client)

class CActiveSlot : public CQuickSlot
{
public:
	enum class SLOTTYPE {POSIONT, ACTIVE, EMOTICON, END};
private:
	CActiveSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CActiveSlot(const CActiveSlot& Prototype);
	virtual ~CActiveSlot() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	SLOTTYPE	m_eType = {};
	_uint		m_iItemID = {};
	_uint		m_iNumItem = {};

private:
	HRESULT Add_Components();

public:
	static CActiveSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
