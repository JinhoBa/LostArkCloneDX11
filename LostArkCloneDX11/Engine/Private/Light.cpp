#include "Light.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight()
	:m_bEnable{ true }
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

void CLight::Update_Position(_float3* pPosition)
{
	memcpy(&m_LightDesc.vPosition, pPosition, sizeof(_float3));
}

void CLight::Update_Range(_float fRange)
{
	m_LightDesc.fRange = fRange;
}

void CLight::Update_Color(_uint iColorType, _float4* pColor)
{
	switch (iColorType)
	{
	case 0:
		memcpy(&m_LightDesc.vDiffuse, pColor, sizeof(_float4));
		break;
	case 1:
		memcpy(&m_LightDesc.vAmbient, pColor, sizeof(_float4));
		break;
	case 2:
		memcpy(&m_LightDesc.vSpecular, pColor, sizeof(_float4));
		break;
	}
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer* pVIBuffer)
{
	_uint iPassIndex = { 0 };

	switch (m_LightDesc.eType)
	{
	case LIGHT::DIRECTIONAL:

		iPassIndex = 1;

		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;

		break;

	case LIGHT::POINT:

		iPassIndex = 2;

		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;

		break;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	return pVIBuffer->Render();
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
