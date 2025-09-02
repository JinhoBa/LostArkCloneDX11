#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct PartObject_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		CTransform* pParentTransform;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

protected:
	CTransform*		m_pParentTransformCom = { nullptr };
	_float4x4		m_CombinedWorldMatrix = {};


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END