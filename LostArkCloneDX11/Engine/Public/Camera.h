#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct Camera_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3 vEye{}, vLookAt{}, vDirection;
		_float fNear{}, fFar{}, fFovy{};
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;
	
public:
	_float	Get_Fovy() const { return m_fFovy; }
	_float Get_Far() const { return m_fFar; }
	_vector Get_TargetPosition() const{ 
		return XMVectorSetW(XMLoadFloat3(&m_vTargetPosition), 1.f);
	}
	void Set_CameraTargetBone(_float4x4* pCameraTargetBoneMatrix) {
		m_pCameraTargetBoneMatrix = pCameraTargetBoneMatrix;
	}
	void Set_LookDircetion(_fvector vDirection) {
		XMStoreFloat3(&m_vDirection, vDirection);
	}
	void Set_Fovy(_float fAngle);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;

	virtual void		Reset();
	void	Update_Lerp(_float fFov, _fvector vPosition, _fvector vTarget);

protected:
	_float			m_fNear = {};
	_float			m_fFar = {};
	_float			m_fFovy = {};
	_float			m_fAspect = {};

	_float			m_fWinSizeX = {};
	_float			m_fWinSizeY = {};

	_float3			m_vTargetPosition = {};

	_float3			m_vDirection = {};
	_float4x4*		m_pCameraTargetBoneMatrix = { nullptr };

protected:
	void	Bind_Transform();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END