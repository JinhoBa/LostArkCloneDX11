#include "StateMachine.h"

#include "State.h"

CStateMachine::CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{pDevice, pContext}
{
}

CStateMachine::CStateMachine(const CComponent& Prototype)
	: CComponent{Prototype}
{
}
HRESULT CStateMachine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStateMachine::Initialize(void* pArg)
{

	return S_OK;
}

void CStateMachine::Upadte(_float fTimeDelta)
{
	m_pState->Update(fTimeDelta);
}

void CStateMachine::Change_State(CState* pState)
{
	if (nullptr != m_pState)
	{
		m_pState->Exit();
		Safe_Release(m_pState);
	}

	m_pState = pState;

	Safe_AddRef(m_pState);

	m_pState->Enter();
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CStateMachine");
		return nullptr;
	}

	return pInstance;
}

CComponent* CStateMachine::Clone(void* pArg)
{
	CComponent* pInstance = new CStateMachine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CStateMachine");
		return nullptr;
	}

	return pInstance;
}

void CStateMachine::Free()
{
	__super::Free();

	Safe_Release(m_pState);
}