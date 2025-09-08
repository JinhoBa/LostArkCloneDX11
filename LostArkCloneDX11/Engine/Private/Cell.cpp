#include "Cell.h"

CCell::CCell()
{
}

HRESULT CCell::Initialize(_uint iIndex, const _float3* pPoint)
{
	m_iIndex = iIndex;

	memcpy(&m_Points, pPoint, sizeof(_float3) * POINT::END);

	_float3 Lines[LINE::END] = {};

	XMStoreFloat3(&Lines[LINE::AB], XMLoadFloat3(&m_Points[POINT::B]) - XMLoadFloat3(&m_Points[POINT::A]));
	XMStoreFloat3(&Lines[LINE::BC], XMLoadFloat3(&m_Points[POINT::C]) - XMLoadFloat3(&m_Points[POINT::B]));
	XMStoreFloat3(&Lines[LINE::CA], XMLoadFloat3(&m_Points[POINT::A]) - XMLoadFloat3(&m_Points[POINT::C]));

	for (_uint i = 0; i < LINE::END; ++i)
	{
		XMStoreFloat3(&m_Normals[i], XMVector3Normalize(XMVectorSet(Lines[i].z, 0.f, Lines[i].x, 0.f)));
	}

	return S_OK;
}

CCell* CCell::Create(_uint iIndex, const _float3* pPoint)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(iIndex, pPoint)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CCell");
		return nullptr;
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();
}