#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"

#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding{ pDevice, pContext }
{
}

void CBounding_OBB::Set_Desc(_float3& vCenter, _float3& vExtents, _float3 vOrientation)
{
	m_pOriginal_Desc->Center = vCenter;
	m_pOriginal_Desc->Extents = vExtents;

	XMStoreFloat4(&m_pOriginal_Desc->Orientation, XMQuaternionRotationRollPitchYaw(vOrientation.x, vOrientation.y, vOrientation.z));
}

HRESULT CBounding_OBB::Initialize(const void* pArg)
{
	const BOUNDING_OBB_DESC* pDesc = static_cast<const BOUNDING_OBB_DESC*>(pArg);

	_float4 vOrientation = {};
	XMStoreFloat4(&vOrientation,XMQuaternionRotationRollPitchYaw(pDesc->vOrientation.x, pDesc->vOrientation.y, pDesc->vOrientation.z));

	m_pOriginal_Desc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vOrientation);
	m_pDesc = new BoundingOrientedBox(*m_pOriginal_Desc);

	return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
	m_pOriginal_Desc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(COLLIDER eType, CBounding* pTarget)
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

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	pBatch->Begin();

	DX::Draw(pBatch, *m_pDesc, vColor);

	pBatch->End();

	return S_OK;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const void* pArg)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBounding_OBB");
		return nullptr;
	}

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();
}
