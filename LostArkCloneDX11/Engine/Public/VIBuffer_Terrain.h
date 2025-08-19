#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual HRESULT Initialize(void* pArg);

	_bool Picking(class CTransform* pTransform, _float3* pOut);

private:
	_uint m_iNumVerticesX = {};
	_uint m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, _uint iSizeX, _uint iSizeZ);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END