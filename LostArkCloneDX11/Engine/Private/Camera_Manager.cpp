#include "Camera_Manager.h"

#include "Camera.h"

CCamera_Manager::CCamera_Manager()
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

HRESULT CCamera_Manager::Bind_Camera(const _wstring& strCameraNameTag, _bool isReturn)
{
	CCamera* pObejct = Find_Camera(strCameraNameTag);

	if (nullptr == pObejct)
		return E_FAIL;

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

	return S_OK;
}

void CCamera_Manager::Priority_Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentCamera)
		m_pCurrentCamera->Priority_Update(fTimeDelta);
}

void CCamera_Manager::Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentCamera)
		m_pCurrentCamera->Update(fTimeDelta);
}

void CCamera_Manager::Late_Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentCamera)
		m_pCurrentCamera->Late_Update(fTimeDelta);
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
