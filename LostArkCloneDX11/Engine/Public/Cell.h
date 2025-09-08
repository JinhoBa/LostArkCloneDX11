#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCell : public CBase
{
public:
	enum POINT { A, B, C, END };
	enum LINE { AB, BC, CA, END };

private:
	CCell();
	virtual ~CCell() = default;

public:
	HRESULT Initialize(_uint iIndex, const _float3* pPoint);

private:
	_uint			m_iIndex = {};
	_float3			m_Points[POINT::END] = {};
	_float3			m_Normals[LINE::END] = {};

public:
	static CCell* Create(_uint iIndex, const _float3* pPoint);
	virtual void Free();
};

NS_END