#include "Light.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

CLight* CLight::Create(const LIGHT_DESC& LightDesc)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CLight");
		return nullptr;
	}

	return pInstance;
}

void CLight::Free()
{
	__super::Free();
}
