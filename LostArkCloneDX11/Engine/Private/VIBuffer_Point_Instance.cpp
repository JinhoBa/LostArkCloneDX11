#include "VIBuffer_Point_Instance.h"

#include "GameInstance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(CVIBuffer_Point_Instance& Prototype)
	:CVIBuffer_Instance{ Prototype },
	m_pInstanceVertices{Prototype.m_pInstanceVertices }
	, m_isLoop{Prototype.m_isLoop },
	m_pSpeed{Prototype.m_pSpeed},
	m_vPivot{Prototype.m_vPivot},
	m_vSize{Prototype.m_vSize},
	m_iMaxInstance{Prototype.m_iMaxInstance}
{
}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc)
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
	const POINT_INSTANCE_DESC* pDesc = static_cast<const POINT_INSTANCE_DESC*>(pInstanceDesc);

	m_isLoop = pDesc->isLoop;
	m_iMaxInstance = m_iNumInstance = pDesc->iNumInstance;
	m_vPivot = pDesc->vPivot;
	m_iInstanceStride = sizeof(VTX_INSTANCE_PARTICLE);
	m_iNumIndexPerInstance = 1;

	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTX_INSTANCE_PARTICLE[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTX_INSTANCE_PARTICLE) * m_iNumInstance);

	m_pSpeed = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeed, sizeof(_float) * m_iNumInstance);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float			fScale = m_pGameInstance->Random(pDesc->vSize.x, pDesc->vSize.y);

		m_vSize = pDesc->vSize;

		m_pInstanceVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		m_pInstanceVertices[i].vTranslation = _float4(
			m_pGameInstance->Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
			1.f);

		m_pInstanceVertices[i].vLifeTime = _float2(0.0f, m_pGameInstance->Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y));

		m_pSpeed[i] = m_pGameInstance->Random(pDesc->vSpeed.x, pDesc->vSpeed.y);
	}

	m_InstanceSubResourceData.pSysMem = m_pInstanceVertices;

#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Bind_Resources()
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

HRESULT CVIBuffer_Point_Instance::Render()
{
	m_pContext->DrawInstanced(1, m_iNumInstance, 0, 0);

	return S_OK;
}

void CVIBuffer_Point_Instance::Set_Desc(_bool isLoop, _float2 vSize, _float2 vLifeTime)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	m_isLoop = isLoop;

	m_vSize = vSize;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		//pVertices[i].vTranslation = _float4(vCenter.x, vCenter.y, vCenter.z, 1.f);
		pVertices[i].vLifeTime.y =  vLifeTime.y;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Set_NumInstance(_uint iNumInstance)
{
	if (m_iMaxInstance < iNumInstance)
		return;

	m_iNumInstance = iNumInstance;
}

void CVIBuffer_Point_Instance::Update(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pVertices[i].vLifeTime.x += fTimeDelta;

		_float fScale = m_vSize.x;

		pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);

		if(true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Scaling(_float fTimeDelta, LERP eLerpType, _float3 vPivot, _float fSpeed)
{
	m_vPivot = vPivot;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	_float fScale = {};

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pVertices[i].vLifeTime.x += fTimeDelta;

		_float fLerpValue = pVertices[i].vLifeTime.x / pVertices[i].vLifeTime.y;

		if (1.f < fLerpValue)
			fLerpValue = 1.f;

		switch (eLerpType)
		{
		case Engine::LERP::LINEAR:
			fScale = Lerp(m_vSize.x, m_vSize.y, fLerpValue);
			break;
		case Engine::LERP::EASEIN:
			fScale = Lerp(m_vSize.x, m_vSize.y, fLerpValue * fLerpValue);
			break;
		case Engine::LERP::EASEOUT:
			fScale = Lerp(m_vSize.x, m_vSize.y, 1.f - (1.f - fLerpValue) * (1.f - fLerpValue));
			break;
		case Engine::LERP::EASEINOUT:
			fScale = Lerp(m_vSize.x, m_vSize.y, fLerpValue < 0.5f ? 2.f * fLerpValue * fLerpValue : 1.f - (-2.f * fLerpValue + 2.f) * (-2.f * fLerpValue + 2.f) / 2.f);
			break;
		}

		pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);

		_vector vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * fSpeed * fTimeDelta);

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;

			pVertices[i].vRight = _float4(m_vSize.x, 0.f, 0.f, 0.f);
			pVertices[i].vUp = _float4(0.f, m_vSize.x, 0.f, 0.f);
			pVertices[i].vLook = _float4(0.f, 0.f, m_vSize.x, 0.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Set_Spread(_bool isLoop, _float3& vPosition, _float3 vPivot, _float3 vRange, _float2 vLifeTime, _float2 vSpeed, _float2 vSize)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	m_isLoop = isLoop;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float			fScale = m_pGameInstance->Random(vSize.x, vSize.y);

		m_vSize = vSize;

		pVertices[i].vTranslation = _float4(
			m_pGameInstance->Random(vPosition.x - vRange.x * 0.5f, vPosition.x + vRange.x * 0.5f),
			m_pGameInstance->Random(vPosition.y - vRange.y * 0.5f, vPosition.y + vRange.y * 0.5f),
			m_pGameInstance->Random(vPosition.z - vRange.z * 0.5f, vPosition.z + vRange.z * 0.5f),
			1.f);

		m_vPivot = vPivot;

		_vector vLook = XMVector3Normalize(XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f));
		_vector vUp = XMVector3Cross(vLook, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		_vector vRight = XMVector3Cross(vUp, vLook);

		XMStoreFloat4(&pVertices[i].vRight, vRight * fScale);
		XMStoreFloat4(&pVertices[i].vUp, vUp * fScale);
		XMStoreFloat4(&pVertices[i].vLook, vLook * fScale);

		pVertices[i].vLifeTime = _float2(0.0f, m_pGameInstance->Random(vLifeTime.x, vLifeTime.y));

		m_pSpeed[i] = m_pGameInstance->Random(vSpeed.x, vSpeed.y);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Set_Circle(_bool isLoop, _float3& vPosition, _float3 vPivot, _float3 vRange, _float2 vLifeTime, _float2 vSpeed, _float2 vSize)
{
	m_isLoop = isLoop;
	m_vPivot = vPivot;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float			fScale = m_pGameInstance->Random(vSize.x, vSize.y);

		pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		pVertices[i].vLifeTime = _float2(0.0f, m_pGameInstance->Random(vLifeTime.x, vLifeTime.y));
		pVertices[i].vTranslation = _float4(
			vPosition.x + sinf(i * 2.f * XM_PI / (_float)m_iNumInstance) * vRange.x,
			vPosition.y,
			vPosition.z + cosf(i * 2.f * XM_PI / (_float)m_iNumInstance) * vRange.x,
			1.f);

		m_pSpeed[i] = m_pGameInstance->Random(vSpeed.x, vSpeed.y);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Set_Corn(_bool isLoop, _float3& vPosition, _float3 vPivot, _float3 vRange, _float2 vLifeTime, _float2 vSpeed, _float2 vSize)
{
	m_isLoop = isLoop;
	m_vPivot = vPivot;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float	fScale = m_pGameInstance->Random(vSize.x, vSize.y);

		_float	fAngle = (i * vRange.y / (_float)(m_iNumInstance - 1)) + vRange.z;

		pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		pVertices[i].vLifeTime = _float2(0.0f, m_pGameInstance->Random(vLifeTime.x, vLifeTime.y));
		pVertices[i].vTranslation = _float4(
			vPosition.x + sinf(fAngle) * vRange.x,
			vPosition.y,
			vPosition.z + cosf(fAngle) * vRange.x,
			1.f);

		m_pSpeed[i] = m_pGameInstance->Random(vSpeed.x, vSpeed.y);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Set_Round(_bool isLoop, _float3& vPosition, _float3 vPivot, _float3 vRange, _float2 vLifeTime, _float2 vSpeed, _float2 vSize)
{
	m_iRoundIndex = 0;
	m_isLoop = isLoop;
	m_vPivot = vPivot;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float			fScale = m_pGameInstance->Random(vSize.x, vSize.y);

		pVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		pVertices[i].vLifeTime = _float2(0.f, m_pGameInstance->Random(vLifeTime.x, vLifeTime.y));
		pVertices[i].vTranslation = _float4(0.f,-10.f, 0.f, 1.f);

		m_pSpeed[i] = m_pGameInstance->Random(vSpeed.x, vSpeed.y);
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Spread(_float3& vPosition, _float3& vRange, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * fTimeDelta);

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = _float4(
				m_pGameInstance->Random(vPosition.x - vRange.x * 0.5f, vPosition.x + vRange.x * 0.5f),
				m_pGameInstance->Random(vPosition.y - vRange.y * 0.5f, vPosition.y + vRange.y * 0.5f),
				m_pGameInstance->Random(vPosition.z - vRange.z * 0.5f, vPosition.z + vRange.z * 0.5f),
				1.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Up(_float3& vPosition, _float3& vRange, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector vDir = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * fTimeDelta);

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = _float4(
				m_pGameInstance->Random(vPosition.x - vRange.x * 0.5f, vPosition.x + vRange.x * 0.5f),
				m_pGameInstance->Random(vPosition.y - vRange.y * 0.5f, vPosition.y + vRange.y * 0.5f),
				m_pGameInstance->Random(vPosition.z - vRange.z * 0.5f, vPosition.z + vRange.z * 0.5f),
				1.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Round(_float3& vPosition, _float3& vRange, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iRoundIndex + 5; ++i)
	{
		_vector vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * fTimeDelta);

		pVertices[i].vLifeTime.x += fTimeDelta;

		if(pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
			++m_iRoundIndex;
		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = _float4(
				vPosition.x + sinf((i % 5) * 2.f * XM_PI / (_float)(m_iNumInstance / 5)) * vRange.x,
				vPosition.y,
				vPosition.z + cosf((i % 5) * 2.f * XM_PI / (_float)(m_iNumInstance / 5)) * vRange.x,
				1.f);
		}
	}

	

	if (m_iNumInstance - 5 <= m_iRoundIndex)
		m_iRoundIndex = 0;

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Corn(_float3& vPosition, _float3& vRange, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * fTimeDelta);

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			_float	fAngle = (i * vRange.y / (_float)(m_iNumInstance - 1)) + vRange.z;

			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = _float4(
				vPosition.x + sinf(fAngle) * vRange.x,
				vPosition.y,
				vPosition.z + cosf(fAngle) * vRange.x,
				1.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Trail(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation));

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Reset()
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vRight = _float4(m_vSize.x, 0.f, 0.f, 0.f);
			pVertices[i].vUp = _float4(0.f, m_vSize.x, 0.f, 0.f);
			pVertices[i].vLook = _float4(0.f, 0.f, m_vSize.x, 0.f);
			pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}


void CVIBuffer_Point_Instance::Circle(_float3& vPosition, _float3& vRange, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTX_INSTANCE_PARTICLE* pVertices = static_cast<VTX_INSTANCE_PARTICLE*>(SubResource.pData);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * fTimeDelta);

		pVertices[i].vLifeTime.x += fTimeDelta;

		if (true == m_isLoop && pVertices[i].vLifeTime.x >= pVertices[i].vLifeTime.y)
		{
			pVertices[i].vLifeTime.x = 0.f;
			pVertices[i].vTranslation = _float4(
				vPosition.x + sinf(i * 2.f * XM_PI / (_float)m_iNumInstance) * vRange.x,
				vPosition.y ,
				vPosition.z + cosf(i * 2.f * XM_PI / (_float)m_iNumInstance) * vRange.x,
				1.f);
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

_float CVIBuffer_Point_Instance::Lerp(_float fStart, _float fEnd, _float fTime)
{
	return fStart * (1.f - fTime) + fEnd * fTime;
}

CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pInstanceDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CVIBuffer_Point_Instance");
		return nullptr;
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CVIBuffer_Point_Instance");
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if(false == m_isCloned)
	{
		Safe_Delete_Array(m_pInstanceVertices);
		Safe_Delete_Array(m_pSpeed);
	}
}
