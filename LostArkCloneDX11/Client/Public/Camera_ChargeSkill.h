#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_ChargeSkill final : public CCamera
{
	enum class CAMERASTATE {READY, ZOOMOUT, END};
	
private:
	CCamera_ChargeSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_ChargeSkill(const CCamera_ChargeSkill& Prototype);
	virtual ~CCamera_ChargeSkill() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void 		Reset() override;

private:
	CAMERASTATE			m_eState = { CAMERASTATE::READY };
	_bool				m_isEnd = {};
	_float				m_fScala = {};
	_float				m_fTimeAcc = {};

private:
	void		Update_Camera_Position();

public:
	static CCamera_ChargeSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
