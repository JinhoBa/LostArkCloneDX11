#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(D3DTS eState, _fmatrix Matrix)
{
	XMStoreFloat4x4(&m_TransformStateMatrices[ENUM_TO_INT(eState)], Matrix);
}

const _float4x4* CPipeLine::Get_Transform_Float4x4(D3DTS eState)
{
	return &m_TransformStateMatrices[ENUM_TO_INT(eState)];
}

const _float4x4* CPipeLine::Get_Transform_Float4x4_Inverse(D3DTS eState)
{
	return &m_TransformStateMatrixInverses[ENUM_TO_INT(eState)];
}

_matrix CPipeLine::Get_Transform_Matrix(D3DTS eState)
{
	return XMLoadFloat4x4(&m_TransformStateMatrices[ENUM_TO_INT(eState)]);
}

_matrix CPipeLine::Get_Transform_MatrixInverse(D3DTS eState)
{
	return XMLoadFloat4x4(&m_TransformStateMatrixInverses[ENUM_TO_INT(eState)]);
}

const _float4* CPipeLine::Get_Camera_Position() const
{
	return &m_CameraPosition;
}

const _float4* CPipeLine::Get_Camera_Look() const
{
	return &m_CameraLook;
}

void CPipeLine::Update()
{
	for (_uint i = 0; i < ENUM_TO_INT(D3DTS::END); ++i)
	{
		XMStoreFloat4x4(&m_TransformStateMatrixInverses[i], XMMatrixInverse(nullptr, Get_Transform_Matrix(static_cast<D3DTS>(i))));
	}

	memcpy(&m_CameraPosition, &m_TransformStateMatrixInverses[ENUM_TO_INT(D3DTS::VIEW)].m[3], sizeof(_float4));
	memcpy(&m_CameraLook, &m_TransformStateMatrixInverses[ENUM_TO_INT(D3DTS::VIEW)].m[2], sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine();
}

void CPipeLine::Free()
{
	__super::Free();
}
