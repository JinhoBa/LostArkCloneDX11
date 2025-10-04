#pragma once
#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Line_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagLineInstanceDesc : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_bool		isLoop;
		_float		fWidth;
		_float2		vSpeed;
		_float2		vLifeTime;
	}LINE_INSTANCE_DESC;
private:
	CVIBuffer_Line_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Line_Instance(CVIBuffer_Line_Instance& Prototype);
	virtual ~CVIBuffer_Line_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc) override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Bind_Resources() override;
	virtual HRESULT Render()override;

public:
#ifdef _DEBUG
	void Set_Desc(_bool isLoop, _uint	iNumInstance, _float2 vSize, _float3 vCenter, _float2 vSpeed, _float3 vRange, _float2 vLifeTime, _float3 vPivot);

#endif // _DEBUG
	void Trail(_float fTimeDelta);

private:
	VTX_INSTANCE_PARTICLE* m_pInstanceVertices = { nullptr };

	_bool					m_isLoop = {};
	_float3					m_vPivot = {};
	_float*					m_pSpeed = { nullptr };

public:
	static CVIBuffer_Line_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
