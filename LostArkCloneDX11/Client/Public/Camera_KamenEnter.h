#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_KamenEnter final : public CCamera
{
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
	_uint			m_iAnimationIndex = {};
	_float			m_fTimeAcc = {};

	CAMERA_ANIMATION_DESC m_anim[3] = {};

public:
	static CCamera_KamenEnter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
