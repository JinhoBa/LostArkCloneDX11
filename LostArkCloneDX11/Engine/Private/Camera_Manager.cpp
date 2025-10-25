#include "Camera_Manager.h"

#include "Camera.h"

CCamera_Manager::CCamera_Manager()
	:m_isLerp{ false }
{
}

CCamera* CCamera_Manager::Find_Camera(const _wstring& strCameraNameTag)
{
	auto iter = m_Cameras.find(strCameraNameTag);

	if (iter == m_Cameras.end())
		return nullptr;

	return (*iter).second;
}

HRESULT CCamera_Manager::Add_Camera(const _wstring& strCameraNameTag, CCamera* pCamera)
{
	CCamera* pObejct = Find_Camera(strCameraNameTag);

	if (nullptr != pObejct)
		return E_FAIL;

	m_Cameras.emplace(strCameraNameTag, pCamera);

	return S_OK;
}

HRESULT CCamera_Manager::Bind_Camera(const _wstring& strCameraNameTag, _bool isReturn, _float fLerpTime)
{
	CCamera* pObejct = Find_Camera(strCameraNameTag);

	if (nullptr == pObejct)
		return E_FAIL;

	m_fLerpTime = fLerpTime;

	if(nullptr != m_pCurrentCamera)
	{
		XMStoreFloat3(&m_vPrePosition, m_pCurrentCamera->Get_Transform()->Get_Position());
		XMStoreFloat3(&m_vPreTargetPosition, m_pCurrentCamera->Get_TargetPosition());
		m_fPreFov = m_pCurrentCamera->Get_Fovy();
	}

	if (false == isReturn)
	{
		m_pPreCamera = m_pCurrentCamera;

		m_pCurrentCamera = pObejct;

		m_pCurrentCamera->Reset();
	}
	else
	{
		m_pCurrentCamera = m_pPreCamera;

		m_pCurrentCamera->Reset();
	}

	if(nullptr != m_pCurrentCamera)
	{
		XMStoreFloat3(&m_vCurPosition, m_pCurrentCamera->Get_Transform()->Get_Position());
		XMStoreFloat3(&m_vCurTargetPosition, m_pCurrentCamera->Get_TargetPosition());
		m_fCurFov = m_pCurrentCamera->Get_Fovy();
	}

	return S_OK;
}

_float* CCamera_Manager::Get_Far()
{
	return m_pCurrentCamera->Get_Far();
}

void CCamera_Manager::Priority_Update(_float fTimeDelta)
{
	if (0.f != m_fLerpTime)
		Lerp_Camera(fTimeDelta);
	else
	{
		if (nullptr != m_pCurrentCamera)
			m_pCurrentCamera->Priority_Update(fTimeDelta);
	}
}

void CCamera_Manager::Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentCamera && !m_isLerp)
		m_pCurrentCamera->Update(fTimeDelta);
}

void CCamera_Manager::Late_Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentCamera && !m_isLerp)
		m_pCurrentCamera->Late_Update(fTimeDelta);
}

void CCamera_Manager::Lerp_Camera(_float fTimeDelta)
{
	m_isLerp = true;
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= m_fLerpTime)
	{
		m_fLerpTime = 0.f;
		m_pCurrentCamera->Update_Lerp(m_fCurFov, XMLoadFloat3(&m_vCurPosition), XMLoadFloat3(&m_vCurTargetPosition));
		m_isLerp = false;

		if (nullptr != m_pCurrentCamera)
			m_pCurrentCamera->Priority_Update(fTimeDelta);

		return;
	}

	_float fLerpValue = (m_fTimeAcc / m_fLerpTime) * (m_fTimeAcc / m_fLerpTime);

	_float fFov = m_fPreFov * (1.f - fLerpValue) + m_fCurFov * m_fTimeAcc / m_fLerpTime;

	_vector vPosition = XMVectorSetW(XMVectorLerp(
		XMLoadFloat3(&m_vPrePosition), XMLoadFloat3(&m_vCurPosition), fLerpValue), 1.f);

	_vector vTarget = XMVectorSetW(XMVectorLerp(
		XMLoadFloat3(&m_vPreTargetPosition), XMLoadFloat3(&m_vCurTargetPosition), fLerpValue), 1.f);

	m_pCurrentCamera->Update_Lerp(fFov, vPosition, vTarget);
}

CCamera_Manager* CCamera_Manager::Create()
{
	return new CCamera_Manager();
}

void CCamera_Manager::Free()
{
	__super::Free();

	for (auto& pair : m_Cameras)
		Safe_Release(pair.second);
	m_Cameras.clear();
}
