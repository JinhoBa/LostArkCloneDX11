#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderTarget_Manager final : public CBase
{
private:
	CRenderTarget_Manager(ID3D11Device pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget_Manager() = default;

public:
	HRESULT Initilize();

public:
	static CRenderTarget_Manager* Create(ID3D11Device pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END