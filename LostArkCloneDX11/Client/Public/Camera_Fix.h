#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Engine)
class CModel;
NS_END

NS_BEGIN(Client)

class CCamera_Fix final : public CCamera
{
private:
	CCamera_Fix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Fix(const CCamera_Fix& Prototype);
	virtual ~CCamera_Fix() = default;

public:
	void Set_CameraTargetPosition(_vector TargetPosition);
	void Set_LookDircetion(_fvector vDirection);
	void Set_State(CAMERA_ANIM eState) { m_eCurState = eState; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	CAMERA_ANIM		m_ePreState = {};
	CAMERA_ANIM		m_eCurState = {};

	_uint			m_iCameraBoneIndex = {};

	_float			m_fDuration = {};
	_float          m_fTimeAcc = {};

	_float3			m_Default_Direction = {};
	_float3			m_vDistance = {};

	_float4			m_pTargetPosition = {};

private:
	void		Update_Camera_Position();
	void		Change_State();

public:
	static CCamera_Fix* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
