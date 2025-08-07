#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CPipeLine final : public CBase
{
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void				Set_Transform(D3DTS eState, _fmatrix Matrix);

	const _float4x4*	Get_Transfrom_Float4x4(D3DTS eState) const;
	_matrix				Get_Transfrom_Matrix(D3DTS eState);
	_matrix				Get_Transfrom_MatrixInverse(D3DTS eState);
	const _float4*		Get_Camera_Position() const;

public:
	void				Update();

private:
	_float4		m_CameraPosition = {};

	_float4x4	m_TransfromStateMatrices[ENUM_TO_INT(D3DTS::END)] = {};
	_float4x4	m_TransfromStateMatrixInverses[ENUM_TO_INT(D3DTS::END)] = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

NS_END