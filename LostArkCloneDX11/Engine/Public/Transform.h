#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct Transform_Desc
	{
		_float fSpeedPersec;
		_float fRotatePersec;
	}TRANSFORM_DESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) const	{
		return XMLoadFloat4(reinterpret_cast<const _float4*>(&m_WorldMatrix.m[ENUM_TO_INT(eState)]));
	}

	_vector Get_Position() const{
		return Get_State(STATE::POSITION);
	}

	_float3 Get_Scale() const;
	void Set_State(STATE eState, _fvector vState)	{
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[ENUM_TO_INT(eState)]), vState);
	}

	void Set_Scale(_float3 vScale);

	_float4x4& Get_WorldMatrix() { return m_WorldMatrix; }
	_float4x4& Get_WorldMatrixInv();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void MoveTo(_float fTimeDelta, _fvector vTargetPos);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation(_float fRadianX, _float fRadianY, _float fRadianZ);
	void LookAt(_fvector vTarget);

private:
	_float		m_fSpeedPersec = {};
	_float		m_fRotatePersec = {};
	_float4x4	m_WorldMatrix = {};
	_float4x4	m_WorldMatrixInv = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END