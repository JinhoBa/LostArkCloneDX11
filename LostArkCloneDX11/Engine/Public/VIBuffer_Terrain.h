#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

	_float2 Get_Size() { return _float2((_float)m_iNumVerticesX, (_float)m_iNumVerticesZ); }

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual HRESULT Initialize(void* pArg);

	_bool	Picking(class CTransform* pTransform, _float3* pOut);
	HRESULT	Change_Verices(_uint iSizeX, _uint iSizeZ);

private:
	_bool m_bChange = {};
	_uint m_iNumVerticesX = {};
	_uint m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END