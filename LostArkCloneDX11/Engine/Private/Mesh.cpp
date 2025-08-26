#include "Mesh.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice, pContext}
{
}

CMesh::CMesh(CMesh& Prototype)
    : CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(MODEL eModelType, const aiMesh* pAIMesh)
{
	HRESULT hr = MODEL::NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(pAIMesh) : Ready_VertexBuffer_For_Anim(pAIMesh);

	if (FAILED(hr))
		return E_FAIL;

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC IBDesc = {};
	IBDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iIndices = {};

	for (_uint i = 0; i < pAIMesh->mNumFaces; ++i)
	{
		pIndices[iIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	D3D11_SUBRESOURCE_DATA InitIBData = {};
	InitIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitIBData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(MODEL eModelType, ifstream& stream)
{
	m_iNumVertexBuffers = 1;
	_uint iNumFaces = {};

	stream.read(reinterpret_cast<_char*>(&m_iMaterialIndex), sizeof(_uint));
	stream.read(reinterpret_cast<_char*>(&m_iNumVertices), sizeof(_uint));
	stream.read(reinterpret_cast<_char*>(&iNumFaces), sizeof(_uint));

	HRESULT hr = 
		MODEL::NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim_Binary(iNumFaces, stream) : Ready_VertexBuffer_For_Anim_Binary(iNumFaces, stream);

	if (FAILED(hr))
		return E_FAIL;

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC IBDesc = {};
	IBDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iIndices = {};

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		stream.read(reinterpret_cast<_char*>(&pIndices[iIndices++]), sizeof(_uint));
		stream.read(reinterpret_cast<_char*>(&pIndices[iIndices++]), sizeof(_uint));
		stream.read(reinterpret_cast<_char*>(&pIndices[iIndices++]), sizeof(_uint));
	}

	D3D11_SUBRESOURCE_DATA InitIBData = {};
	InitIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitIBData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Save_To_Binary(MODEL eModelType, const aiMesh* pAIMesh, ofstream& stream)
{
	m_iNumVertices = pAIMesh->mNumVertices;

#pragma region VETEX_BUFFER

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_iNumBones = pAIMesh->mNumBones;

	if(MODEL::ANIM == eModelType)
	{
		for (_uint i = 0; i < m_iNumBones; ++i)
		{
			aiBone* pBone = pAIMesh->mBones[i];

			if (nullptr == pBone)
				return E_FAIL;

			for (_uint j = 0; j < pBone->mNumWeights; ++j)
			{
				aiVertexWeight Weight = pBone->mWeights[j];

				if (0.f == pVertices[Weight.mVertexId].vBlendWeight.x)
				{
					pVertices[Weight.mVertexId].vBlendIndex.x = i;
					pVertices[Weight.mVertexId].vBlendWeight.x = Weight.mWeight;
				}
				else if (0.f == pVertices[Weight.mVertexId].vBlendWeight.y)
				{
					pVertices[Weight.mVertexId].vBlendIndex.y = i;
					pVertices[Weight.mVertexId].vBlendWeight.y = Weight.mWeight;
				}
				else if (0.f == pVertices[Weight.mVertexId].vBlendWeight.z)
				{
					pVertices[Weight.mVertexId].vBlendIndex.z = i;
					pVertices[Weight.mVertexId].vBlendWeight.z = Weight.mWeight;
				}
				else
				{
					pVertices[Weight.mVertexId].vBlendIndex.w = i;
					pVertices[Weight.mVertexId].vBlendWeight.w = Weight.mWeight;
				}
			}
		}
	}

	if(MODEL::ANIM == eModelType)
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
		{
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vPosition), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vNormal), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vTangent), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vTexcoord), sizeof(_float2));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vBlendIndex), sizeof(_float4));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vBlendWeight), sizeof(_float4));
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
		{
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vPosition), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vNormal), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vTangent), sizeof(_float3));
			stream.write(reinterpret_cast<const _char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		}
	}


	Safe_Delete_Array(pVertices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VETEX_BUFFER
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}


	D3D11_SUBRESOURCE_DATA InitVBData = {};
	InitVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXANIMMESH);

	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VETEX_BUFFER
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pBone = pAIMesh->mBones[i];

		if (nullptr == pBone)
			return E_FAIL;

		for (_uint j = 0; j < pBone->mNumWeights; ++j)
		{
			aiVertexWeight Weight = pBone->mWeights[j];

			if (0.f == pVertices[Weight.mVertexId].vBlendWeight.x)
			{
				pVertices[Weight.mVertexId].vBlendIndex.x = i;
				pVertices[Weight.mVertexId].vBlendWeight.x = Weight.mWeight;
			}
			else if (0.f == pVertices[Weight.mVertexId].vBlendWeight.y)
			{
				pVertices[Weight.mVertexId].vBlendIndex.y = i;
				pVertices[Weight.mVertexId].vBlendWeight.y = Weight.mWeight;
			}
			else if (0.f == pVertices[Weight.mVertexId].vBlendWeight.z)
			{
				pVertices[Weight.mVertexId].vBlendIndex.z = i;
				pVertices[Weight.mVertexId].vBlendWeight.z = Weight.mWeight;
			}
			else
			{
				pVertices[Weight.mVertexId].vBlendIndex.w = i;
				pVertices[Weight.mVertexId].vBlendWeight.w = Weight.mWeight;
			}
		}
	}


	D3D11_SUBRESOURCE_DATA InitVBData = {};
	InitVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim_Binary(_uint iNumFames, ifstream& stream)
{
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = iNumFames * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VETEX_BUFFER
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vPosition), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vNormal), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vTangent), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vTexcoord), sizeof(_float2));
	}

	D3D11_SUBRESOURCE_DATA InitVBData = {};
	InitVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim_Binary(_uint iNumFames, ifstream& stream)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	m_iNumIndices = iNumFames * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VETEX_BUFFER
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vPosition), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vNormal), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vTangent), sizeof(_float3));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vBlendIndex), sizeof(_float4));
		stream.read(reinterpret_cast<_char*>(&pVertices[i].vBlendWeight), sizeof(_float4));
	}

	D3D11_SUBRESOURCE_DATA InitVBData = {};
	InitVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitVBData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const aiMesh* pAIMesh)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMesh");
		return nullptr;
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, ifstream& stream)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, stream)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMesh");
		return nullptr;
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CComponent* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CMesh");
		return nullptr;
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();
}
