#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer(CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_Resources(ID3D11InputLayout* pLayout);
	HRESULT Render();

protected:
	ID3D11Buffer*				m_pVB = { nullptr };
	ID3D11Buffer*				m_pIB = { nullptr };

	_float3*					m_pVertexPositions = { nullptr };

	_uint						m_iNumVertices = {};
	_uint						m_iVertexStride = {};

	_uint						m_iNumIndices = {};
	_uint						m_iIndexStride = {};

	_uint						m_iNumVertexBuffers = {};
	DXGI_FORMAT					m_eIndexFormat = {};
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive = {};

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END