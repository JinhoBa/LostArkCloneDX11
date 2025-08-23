#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	typedef struct VIBuffeer_Terrain_Desc
	{
		_uint iSizeX{}, iSizeZ;
		_float fPosY{};
	}VIBUFFER_TERRAIN_DESC;
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

	_float2 Get_Size() { return _float2((_float)m_iNumVerticesX, (_float)m_iNumVerticesZ); }

public:
	virtual HRESULT Initialize_Prototype(const _char* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual HRESULT Initialize(void* pArg);

	HRESULT Save_HeightFile(const _char* pFilePath);
	HRESULT Load_HeightFile(const _char* pFilePath);

	HRESULT Update_Vertex();

	_bool	Picking(class CTransform* pTransform, _float3* pOut);
	_bool	Picking_Edit(class CTransform* pTransform, _float3* pOut, _float fY);
	_bool	Picking_Smooth(class CTransform* pTransform, _float3* pOut, _float fY);
	HRESULT	Change_Verices(_uint iSizeX, _uint iSizeZ);

private:
	_bool				m_bChange = {};
	_uint				m_iNumVerticesX = {};
	_uint				m_iNumVerticesZ = {};

	vector<_float>		m_HeigthValues = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END