#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CShadow final : public CBase
{
private:
	CShadow();
	virtual ~CShadow() = default;

public:
	HRESULT Ready_Shadow_Light(const SHADOW_LIGHT_DESC& Desc);
	HRESULT Bind_Shadow_Resource(class CShader* pShader, const _char* pContantName, D3DTS eType) const;

private:
	_float4x4 m_TransformationMatrices[ENUM_TO_INT(D3DTS::END)] = {};

public:
	static CShadow* Create();
	virtual void Free() override;
};

NS_END