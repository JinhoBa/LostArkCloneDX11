#pragma once
#include "Component.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	void Set_OnCollisionEnter(function<void()>Event) {
		m_OnCollisionEnter_Event = Event;
	}
	void Set_OnCollisionStay(function<void()>Event) {
		m_OnCollisionStay_Event = Event;
	}
	void Set_OnCollisionExit(function<void()>Event) {
		m_OnCollisionExit_Event = Event;
	}
	void Hit() { m_isColl = true; };


public:
	virtual HRESULT		Initialize_Prototype(COLLIDER eType);
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Update(_fmatrix WorldMatrix);

	_bool				Intersect(CCollider* pTarget);
	void				Update_OnCollision();


#ifdef _DEBUG
public:
	virtual HRESULT	Render();
#endif 

private:
	_bool				m_PreisColl = { false };
	_bool				m_isColl = { false };
	COLLIDER			m_eType = { COLLIDER::END };

	class CBounding*	m_pBounding = { nullptr };

	function<void()>	m_OnCollisionEnter_Event = { nullptr };
	function<void()>	m_OnCollisionStay_Event = { nullptr };
	function<void()>	m_OnCollisionExit_Event = { nullptr };

#ifdef _DEBUG
public:
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*									m_pEffect = { nullptr };
	ID3D11InputLayout*								m_pInputLayout = { nullptr };
#endif 


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END