#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CNavigation : public CComponent
{
public:
	typedef struct Navigation_Desc
	{
		_uint iCurrentIndex{ -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CComponent& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* pNavigaitonFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Upadte(_float fTimeDelta);

private:
	_uint					m_iCurrentIndex = {};

	vector<class CCell*>	m_Cells;

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pNavigaitonFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END