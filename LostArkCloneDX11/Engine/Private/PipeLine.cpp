#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(D3DTS eState, _fmatrix Matrix)
{
	XMStoreFloat4x4(&m_TransfromStateMatrices[ENUM_TO_INT(eState)], Matrix);
}

const _float4x4* CPipeLine::Get_Transfrom_Float4x4(D3DTS eState) const
{
	return &m_TransfromStateMatrices[ENUM_TO_INT(eState)];
}

_matrix CPipeLine::Get_Transfrom_Matrix(D3DTS eState)
{
	return XMLoadFloat4x4(&m_TransfromStateMatrices[ENUM_TO_INT(eState)]);
}

_matrix CPipeLine::Get_Transfrom_MatrixInverse(D3DTS eState)
{
	return XMLoadFloat4x4(&m_TransfromStateMatrixInverses[ENUM_TO_INT(eState)]);
}

const _float4* CPipeLine::Get_Camera_Position() const
{
	return &m_CameraPosition;
}

void CPipeLine::Update()
{
	for (_uint i = 0; i < ENUM_TO_INT(D3DTS::END); ++i)
	{
		XMStoreFloat4x4(&m_TransfromStateMatrixInverses[i], XMMatrixInverse(nullptr, Get_Transfrom_Matrix(static_cast<D3DTS>(i))));
	}
	
	memcpy(&m_CameraPosition, &m_TransfromStateMatrixInverses[ENUM_TO_INT(D3DTS::VIEW)].m[3], sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine();
}

void CPipeLine::Free()
{
	__super::Free();
}
