#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CCamera_Manager final : public CBase
{
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	class CCamera* Find_Camera(const _wstring& strCameraNameTag);

	HRESULT Add_Camera(const _wstring& strCameraNameTag, class CCamera* pGameObject);
	HRESULT Bind_Camera(const _wstring& strCameraNameTag, _bool isReturn, _float fLerpTime);

public:
	void	Priority_Update(_float fTimeDelta);
	void	Update(_float fTimeDelta);
	void	Late_Update(_float fTimeDelta);

private:
	class CCamera*		m_pCurrentCamera = { nullptr };
	class CCamera*		m_pPreCamera = { nullptr };

	_bool				m_isLerp = {};

	_float				m_fLerpTime = {};
	_float				m_fTimeAcc = {};
	_float				m_fPreFov = {};
	_float				m_fCurFov = {};

	_float3				m_vPrePosition = {};
	_float3				m_vCurPosition = {};

	_float3				m_vPreTargetPosition = {};
	_float3				m_vCurTargetPosition = {};

	map<const _wstring, class CCamera*>			m_Cameras;

private:
	void Lerp_Camera(_float fTimeDelta);

public:
	static CCamera_Manager* Create();
	virtual void Free() override;
};

NS_END