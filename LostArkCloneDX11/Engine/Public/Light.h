#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHT_DESC* Get_LightDesc() { return &m_LightDesc; }
	_bool isEnalbe() const { return m_bEnable; }
	void ToggleLight(_bool bEnable) { m_bEnable = bEnable; }

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	void Update_Position(_float3* pPosition);
	void Update_Range(_float fRange);
	void Update_Color(_uint iColorType, _float4* pColor);
	HRESULT Render(class CShader* pShader, class CVIBuffer* pVIBuffer);


private:
	_bool		m_bEnable = {};

	LIGHT_DESC	m_LightDesc = {};

private:
	

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

NS_END