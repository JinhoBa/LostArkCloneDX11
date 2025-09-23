#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundingDesc
	{
		COLLIDERTYPE		eColliderType{};
		_float3				vCenter;
		class CGameObject*  pOwner;
	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual _float3& Get_WorldPosition() PURE;
	

public:
	HRESULT			Initialize();
	virtual void	Update(_fmatrix WorldMatrix) PURE;
	virtual _bool	Intersect(COLLIDER eType, class CBounding* pTarget) PURE;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) PURE;
#endif

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free();
};

NS_END