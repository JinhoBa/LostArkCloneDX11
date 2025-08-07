#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct Camera_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3 vEye{}, vLookAt{};
		_float fNear{}, fFar{}, fFovy{};
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;

protected:
	_float	m_fNear = {};
	_float	m_fFar = {};
	_float	m_fFovy = {};
	_float	m_fAspect = {};

	_float	m_fWinSizeX = {};
	_float	m_fWinSizeY = {};

protected:
	void	Bind_Transform();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END