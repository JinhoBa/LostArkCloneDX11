#include "Cell.h"

#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{ pDevice }, m_pContext{pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

const _bool CCell::isInCell(_vector vPositon, _int* pNeighborIndex) const
{
	for (_uint i = 0; i < LINE::LINE_END; ++i)
	{
		_vector vDir = XMVector3Normalize(vPositon - XMLoadFloat3(&m_Points[i]));

		if (0.f < XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_Normals[i]), vDir)))
		{
			*pNeighborIndex = m_NeighborIndices[i];
			return false;
		}
	}

	*pNeighborIndex = -1;

	return true;
}

const _bool CCell::isNeighbor(_vector vSrcPositon, _vector vDestPositon) const
{
	if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::A]), vSrcPositon))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::B]), vDestPositon))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::C]), vDestPositon))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::B]), vSrcPositon))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::C]), vDestPositon))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::A]), vDestPositon))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::C]), vSrcPositon))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::A]), vDestPositon))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_Points[POINT::B]), vDestPositon))
			return true;
	}

	return false;
}
HRESULT CCell::Initialize(_uint iIndex, const _float3* pPoint)
{
	m_iIndex = iIndex;

	memcpy(&m_Points, pPoint, sizeof(_float3) * POINT::POINT_END);

	_float3 Lines[LINE::LINE_END] = {};

	XMStoreFloat3(&Lines[LINE::AB], XMLoadFloat3(&m_Points[POINT::B]) - XMLoadFloat3(&m_Points[POINT::A]));
	XMStoreFloat3(&Lines[LINE::BC], XMLoadFloat3(&m_Points[POINT::C]) - XMLoadFloat3(&m_Points[POINT::B]));
	XMStoreFloat3(&Lines[LINE::CA], XMLoadFloat3(&m_Points[POINT::A]) - XMLoadFloat3(&m_Points[POINT::C]));

	for (_uint i = 0; i < LINE::LINE_END; ++i)
	{
		XMStoreFloat3(&m_Normals[i], XMVector3Normalize(XMVectorSet(Lines[i].z, 0.f, Lines[i].x, 0.f)));
	}

#ifdef _DEBUG
	m_pVIBufferCom = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_Points);

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;
#endif 

	return S_OK;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}
#endif


CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iIndex, const _float3* pPoint)
{
	CCell* pInstance = new CCell(pDevice, pContext);

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

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pVIBufferCom);
#endif 
}