#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"
#include "Bounding_OBB.h"

#include "DebugDraw.h"

void CBounding_AABB::Set_Desc(_float3& vCenter, _float3& vExtents)
{
	m_pOriginal_Desc->Center = vCenter;
	m_pOriginal_Desc->Extents = vExtents;
}

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding{pDevice, pContext}
{
}

HRESULT CBounding_AABB::Initialize(const void* pArg)
{
	const BOUNDING_AABB_DESC* pDesc = static_cast<const BOUNDING_AABB_DESC*>(pArg);

	m_pOriginal_Desc = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginal_Desc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
	_matrix TransformMatrix = WorldMatrix;

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]);

	m_pOriginal_Desc->Transform(*m_pDesc, TransformMatrix);
}

_bool CBounding_AABB::Intersect(COLLIDER eType, CBounding* pTarget)
{
	_bool isColl = { false };

	switch (eType)
	{
	case Engine::COLLIDER::AABB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
		break;
	case Engine::COLLIDER::OBB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());
		break;
	case Engine::COLLIDER::SPHERE:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
		break;
	}

	return isColl;
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	pBatch->Begin();

	DX::Draw(pBatch, *m_pDesc, vColor);

	pBatch->End();

	return S_OK;
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* pArg)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if(FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBounding_AABB");
		return nullptr;
	}
	
	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginal_Desc);
	Safe_Delete(m_pDesc);
}
