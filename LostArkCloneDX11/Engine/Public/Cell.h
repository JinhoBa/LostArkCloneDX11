#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCell : public CBase
{
public:
	enum POINT { A, B, C, POINT_END };
	enum LINE { AB, BC, CA, LINE_END };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) const { return XMLoadFloat3(&m_Points[ePoint]); }
	void	Set_Neighbor(LINE eLine, CCell* pNeighborCell) { 
		m_NeighborIndices[eLine] = pNeighborCell->m_iIndex;
	}
	const _bool isInCell(_vector vPositon, _int* pNeighborIndex) const;
	const _bool isNeighbor(_vector vSrcPositon, _vector vDestPositon) const;
	_float Compute_Height(_fvector vPosition) const;

public:
	HRESULT Initialize(_uint iIndex, const _float3* pPoint);

#ifdef _DEBUG
	_bool Check_Points(_fvector vPinkingPosition, _float3* pPoint);
	HRESULT Save_Binary(ofstream& out);
	HRESULT Render();
#endif

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_uint					m_iIndex = {};
	_int					m_NeighborIndices[LINE::LINE_END] = { -1, -1, -1 };
	_float3					m_Points[POINT::POINT_END] = {};
	_float3					m_Normals[LINE::LINE_END] = {};

	_float4					m_vPlane = {};

#ifdef _DEBUG
	class CVIBuffer_Cell* m_pVIBufferCom = { nullptr };
#endif 

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iIndex, const _float3* pPoint);
	virtual void Free();
};

NS_END