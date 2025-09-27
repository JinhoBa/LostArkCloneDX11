#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Clash final : public CCamera
{
private:
	CCamera_Clash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Clash(const CCamera_Clash& Prototype);
	virtual ~CCamera_Clash() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		Reset() override;

private:
	void		Update_Camera_Position();
	void		Change_State();

public:
	static CCamera_Clash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
