#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Fix final : public CCamera
{
private:
	CCamera_Fix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Fix(const CCamera_Fix& Prototype);
	virtual ~CCamera_Fix() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		Reset() override;

private:
	_bool		m_isShake = {};

	_float		m_fDuration = {};
	_float		m_fChangeDuration = {};

	_float		m_fTimeAcc = {};

	_float4     m_vPositionOffset = {};

private:
	void		Update_Camera_Position(_float fTimeDelta);
	void		Change_State();
	void		Shake(_float fTimeDelta);


public:
	static CCamera_Fix* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
