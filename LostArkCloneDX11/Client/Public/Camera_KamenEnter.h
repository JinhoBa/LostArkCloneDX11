#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_KamenEnter final : public CCamera
{
	typedef struct tagCameraAnimation
	{
		_bool		isLoop{};
		_float		fDuration{};
		_float      fSpeed{};
		_float		fStartFov{};
		_float		fEndFov{};
		_float     fRotationSpeed{};
		_float3		vStartPositon{};
		_float3		vEndPosition{};
		_float3     vRotationAxis{};
		_float3		vTargetPosition{};
	}CAMERA_ANIMATION;

private:
	CCamera_KamenEnter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_KamenEnter(const CCamera_KamenEnter& Prototype);
	virtual ~CCamera_KamenEnter() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		Reset()override;

private:
	_float			m_fTimeAcc = {};

	_float4			m_vTargetPosition = {};


	CAMERA_ANIMATION m_anim = {};

private:
	void Update_Camera_Position();
	void ZoomIn(_float fTimeDelta);
	void ZoomOut(_float fTimeDelta);

public:
	static CCamera_KamenEnter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
