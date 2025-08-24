#include "Mesh.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice, pContext}
{
}

CMesh::CMesh(CMesh& Prototype)
    : CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(const aiMesh* pAIMesh)
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

HRESULT CMesh::Initialize_Prototype(ifstream& stream)
{
	stream.read(reinterpret_cast<_char*>(&m_iMaterialIndex), sizeof(_uint));
	m_iNumVertexBuffers = 1;
	stream.read(reinterpret_cast<_char*>(&m_iNumVertices), sizeof(_uint));
	m_iVertexStride = sizeof(VTXMESH);

	_uint iNumFaces = {};
	stream.read(reinterpret_cast<_char*>(&iNumFaces), sizeof(_uint));
	m_iNumIndices = iNumFaces * 3;
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

#pragma endregion

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

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIMesh)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMesh");
		return nullptr;
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& stream)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(stream)))
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
