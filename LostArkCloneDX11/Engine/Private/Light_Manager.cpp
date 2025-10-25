#include "Light_Manager.h"

#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

const LIGHT_DESC* CLight_Manager::Get_Desc(const _tchar* pLightTag)
{
	CLight* pLight = Find_Light(pLightTag);

	if (nullptr == pLight)
		return nullptr;

	return pLight->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(const _tchar* pLightTag, const LIGHT_DESC& LightDesc)
{
	if (nullptr != Find_Light(pLightTag))
		return E_FAIL;

	CLight* pLight = CLight::Create(LightDesc);

	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.emplace(pLightTag, pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader* pShader, CVIBuffer* pVIBuffer)
{
	for (auto& pair : m_Lights)
	{
		if (false == pair.second->isEnalbe())
			continue;

		if (FAILED(pair.second->Render(pShader, pVIBuffer)))
			return E_FAIL;
	}

	return S_OK;
}

void CLight_Manager::ToggleLight(const _tchar* pLightTag, _bool bEnable)
{
	auto iter = m_Lights.find(pLightTag);

	if (iter == m_Lights.end())
		return;

	iter->second->ToggleLight(bEnable);
}

void CLight_Manager::Update_Position(const _tchar* pLightTag, _float3* pPosition)
{
	auto iter = m_Lights.find(pLightTag);

	if (iter == m_Lights.end())
		return;

	iter->second->Update_Position(pPosition);
}

void CLight_Manager::Update_Range(const _tchar* pLightTag, _float fRange)
{
	auto iter = m_Lights.find(pLightTag);

	if (iter == m_Lights.end())
		return;

	iter->second->Update_Range(fRange);
}

void CLight_Manager::Update_Color(const _tchar* pLightTag, _uint iColorType, _float4* pColor)
{
	auto iter = m_Lights.find(pLightTag);

	if (iter == m_Lights.end())
		return;

	iter->second->Update_Color(iColorType, pColor);
}

CLight* CLight_Manager::Find_Light(const _tchar* pLightTag)
{
	auto iter = m_Lights.find(pLightTag);

	if (iter == m_Lights.end())
		return nullptr;

	return iter->second;
}

CLight_Manager* CLight_Manager::Create()
{
	return new CLight_Manager();
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Lights)
		Safe_Release(Pair.second);
	m_Lights.clear();
}
