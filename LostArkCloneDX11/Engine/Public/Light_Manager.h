#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLight_Manager : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC& Get_Desc(_uint iLightIndex);

public:
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer* pVIBuffer);

private:
	list<class CLight*> m_Lights;

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

NS_END