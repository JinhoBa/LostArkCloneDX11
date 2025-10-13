#pragma once
#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagPointInstanceDesc : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_bool		isLoop;
		_float3		vPivot;
		_float2		vSpeed;
		_float2		vLifeTime;
	}POINT_INSTANCE_DESC;
private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc) override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Bind_Resources() override;
	virtual HRESULT Render()override;

public:
#ifdef _DEBUG
	void Set_Desc(_bool isLoop, _float2 vSize, _float3 vCenter, _float3 vRange, _float2 vLifeTime);
#endif // _DEBUG
	void Update(_float fTimeDelta);
	void Scaling(_float fTimeDelta, LERP eLerpType, _float3 vPivot, _float fSpeed);
	void Spread(_float fTimeDelta);
	void Trail(_float fTimeDelta);
	void Reset();

private:
	VTX_INSTANCE_PARTICLE*	m_pInstanceVertices = { nullptr };

	_bool					m_isLoop = {};
	_float2					m_vSize = {};
	_float3					m_vPivot = {};
	_float*					m_pSpeed = { nullptr };

private:
	_float Lerp(_float fStart, _float fEnd, _float fTime);

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END