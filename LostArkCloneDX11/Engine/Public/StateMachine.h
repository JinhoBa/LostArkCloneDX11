#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CStateMachine : public CComponent
{
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStateMachine(const CComponent& Prototype);
	virtual ~CStateMachine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Upadte(_float fTimeDelta);

	void Change_State(class CState* pState);

private:
	class CState* m_pState = { nullptr };

public:
	static CStateMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

NS_END