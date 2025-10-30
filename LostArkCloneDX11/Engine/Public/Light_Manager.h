#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLight_Manager : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_Desc(const _tchar* pLightTag);
public:
	HRESULT Add_Light(const _tchar* pLightTag, const LIGHT_DESC& LightDesc);
	HRESULT Delete_Light(const _tchar* pLightTag);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer* pVIBuffer);

	void ToggleLight(const _tchar* pLightTag,_bool bEnable);
	void Update_Position(const _tchar* pLightTag, _float3* pPosition);
	void Update_Range(const _tchar* pLightTag, _float fRange);
	void Update_Color(const _tchar* pLightTag, _uint iColorType, _float4* pColor);

private:
	map<_wstring ,class CLight*> m_Lights;

private:
	class CLight* Find_Light(const _tchar* pLightTag);

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

NS_END