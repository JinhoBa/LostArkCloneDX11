#include "VIBuffer_Line_Instance.h"

#include "GameInstance.h"

CVIBuffer_Line_Instance::CVIBuffer_Line_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Line_Instance::CVIBuffer_Line_Instance(CVIBuffer_Line_Instance& Prototype)
	:CVIBuffer_Instance{ Prototype },
	m_pInstanceVertices{ Prototype.m_pInstanceVertices }
	, m_isLoop{ Prototype.m_isLoop },
	m_pDist{ Prototype.m_pDist }
{
}

HRESULT CVIBuffer_Line_Instance::Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc)
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOS);

	m_iNumIndices = 0;
	m_iIndexStride = 0;

	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VETEX_BUFFER
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	m_pVertexPositions[0] = pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);

	D3D11_SUBRESOURCE_DATA InitVBData = {};
	InitVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INSTANCE_BUFFER
	const LINE_INSTANCE_DESC* pDesc = static_cast<const LINE_INSTANCE_DESC*>(pInstanceDesc);

	m_isLoop = pDesc->isLoop;
	m_iNumInstance = pDesc->iNumInstance;
	m_iInstanceStride = sizeof(VTX_INSTANCE_TRAIL);
	m_iNumIndexPerInstance = 1;

	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTX_INSTANCE_TRAIL[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTX_INSTANCE_TRAIL) * m_iNumInstance);

	m_pDist = new _float[m_iNumInstance];
	ZeroMemory(m_pDist, sizeof(_float) * m_iNumInstance);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pInstanceVertices[i].vStartRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vEndRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vEndPosition = _float4(
			 pDesc->vCenter.x,
			 pDesc->vCenter.y,
			 pDesc->vCenter.z,
			1.f);
		m_pInstanceVertices[i].vStartPosition = _float4(0.f, 0.f, 0.f, 1.f);
		m_pInstanceVertices[i].vLifeTime = _float2(0.0f,  pDesc->vLifeTime.y);
		m_pInstanceVertices[i].vWidth = _float3(pDesc->vWidth.x, pDesc->vWidth.y, pDesc->vWidth.z);

		m_pDist[i] = 0.f;
	}

	m_InstanceSubResourceData.pSysMem = m_pInstanceVertices;

#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Line_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Line_Instance::Bind_Resources()
{
	ID3D11Buffer* VertexBuffers[] = {
		m_pVB,
		m_pVBInstance,
	};

	_uint VertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride,
	};

	_uint Offsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, VertexBuffers, VertexStrides, Offsets);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive);

	return S_OK;
}

HRESULT CVIBuffer_Line_Instance::Render()
{
	m_pContext->DrawInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0);

	return S_OK;
}

void CVIBuffer_Line_Instance::Set_Desc(_bool isLoop, _uint iNumInstance, _float2 vSize, _float3 vCenter, _float2 vSpeed, _float3 vRange, _float2 vLifeTime, _float3 vPivot)
{


}

void CVIBuffer_Line_Instance::Add_Position(_float4x4* pMatrix)
{
	_float fDist = {};

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_TRAIL* pVertices = static_cast<VTX_INSTANCE_TRAIL*>(SubResource.pData);

	if (m_iNumInstance <= m_iNumPosition)
	{
		return;
	}
	++m_iNumPosition;

	/* Position */
	pVertices[m_iNumPosition - 1].vLifeTime.x = 0.f;

	XMStoreFloat4(&pVertices[m_iNumPosition - 1].vStartRight, XMVectorSet(pMatrix->_11, pMatrix->_12, pMatrix->_13, 0.f));
	XMStoreFloat4(&pVertices[m_iNumPosition - 1].vStartPosition, XMVectorSet(pMatrix->_41, pMatrix->_42, pMatrix->_43, 1.f));

	/* 이동 거리 */
	if (0 != m_iNumPosition - 1)
	{
		XMStoreFloat4(&pVertices[m_iNumPosition - 1].vEndRight, XMLoadFloat4(&pVertices[m_iNumPosition - 2].vStartRight));
		XMStoreFloat4(&pVertices[m_iNumPosition - 1].vEndPosition, XMLoadFloat4(&pVertices[m_iNumPosition - 2].vStartPosition));
		pVertices[m_iNumPosition - 1].vWidth.z = pVertices[m_iNumPosition - 2].vWidth.z + XMVectorGetX(XMVector3Length(
			XMLoadFloat4(&pVertices[m_iNumPosition - 1].vStartPosition) - XMLoadFloat4(&pVertices[m_iNumPosition - 2].vStartPosition)));
	}
	else
	{
		XMStoreFloat4(&pVertices[m_iNumPosition - 1].vEndRight, XMVectorSet(pMatrix->_11, pMatrix->_12, pMatrix->_13, 0.f));
		XMStoreFloat4(&pVertices[m_iNumPosition - 1].vEndPosition, XMVectorSet(pMatrix->_41, pMatrix->_42, pMatrix->_43, 1.f));
		pVertices[m_iNumPosition - 1].vWidth.z = 0.f;
	}

	m_pContext->Unmap(m_pVBInstance, 0);

	return;
}

_float CVIBuffer_Line_Instance::Trail(_float fTimeDelta)
{
	_float fDist = 0.f;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_TRAIL* pVertices = static_cast<VTX_INSTANCE_TRAIL*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumPosition; ++i)
	{
		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
		}
	}
	if (m_iNumPosition > 1)
		fDist = pVertices[m_iNumPosition - 1].vWidth.z;

	m_pContext->Unmap(m_pVBInstance, 0);

	return fDist;
}

void CVIBuffer_Line_Instance::Clear()
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_TRAIL* pVertices = static_cast<VTX_INSTANCE_TRAIL*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		XMStoreFloat4(&pVertices[i].vStartRight, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		XMStoreFloat4(&pVertices[i].vEndRight, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		XMStoreFloat4(&pVertices[i].vStartPosition, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		XMStoreFloat4(&pVertices[i].vEndPosition, XMVectorSet(0.f, 0.f, 0.f, 1.f));

		pVertices[i].vLifeTime.x = 0.f;
		pVertices[i].vWidth.z = 0.f;
		m_pDist[i] = 0.f;
	}

	m_iNumPosition = 0;

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Line_Instance* CVIBuffer_Line_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_Line_Instance* pInstance = new CVIBuffer_Line_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pInstanceDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CVIBuffer_Line_Instance");
		return nullptr;
	}

	return pInstance;
}

CComponent* CVIBuffer_Line_Instance::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Line_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CVIBuffer_Line_Instance");
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Line_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pInstanceVertices);
		Safe_Delete_Array(m_pDist);
	}
}
