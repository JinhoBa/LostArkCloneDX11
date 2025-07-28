#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class CUIObject abstract : public CGameObject
{
public:
	typedef struct UIObject_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float fX, fY, fZ, fSizeX, fSizeY;
		CTransform* pParent_TransformCom{};
	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	_float Get_ZValue() const { return m_fZ; }

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	void			Update_Position();

protected:
	_float		m_fX = {};
	_float		m_fY = {};
	_float		m_fZ = {};
	_float		m_fSizeX = {};
	_float		m_fSizeY = {};
	CTransform* m_pParent_TransformCom = { nullptr };	

public:
	virtual CUIObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END