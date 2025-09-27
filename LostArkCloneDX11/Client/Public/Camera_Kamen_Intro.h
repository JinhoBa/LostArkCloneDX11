#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Kamen_Intro final : public CCamera
{
private:
	CCamera_Kamen_Intro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Kamen_Intro(const CCamera_Kamen_Intro& Prototype);
	virtual ~CCamera_Kamen_Intro() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		Reset() override;

private:
	void Update_Camera_Position();

public:
	static CCamera_Kamen_Intro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
