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
	HRESULT Bind_Camera(const _wstring& strCameraNameTag, _bool isReturn = false);

public:
	void	Priority_Update(_float fTimeDelta);
	void	Update(_float fTimeDelta);
	void	Late_Update(_float fTimeDelta);

private:
	class CCamera*								m_pCurrentCamera = { nullptr };
	class CCamera*								m_pPreCamera = { nullptr };
	map<const _wstring, class CCamera*>			m_Cameras;

public:
	static CCamera_Manager* Create();
	virtual void Free() override;
};

NS_END