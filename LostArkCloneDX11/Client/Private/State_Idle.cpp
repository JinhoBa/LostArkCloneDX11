#include "pch.h"
#include "State_Idle.h"

#include "Model.h"
#include "Transform.h"

CState_Idle::CState_Idle()
	:CState{}
{
}

HRESULT CState_Idle::Initilize(CModel* pModel, CTransform* pTransform)
{
	if (nullptr == pModel || nullptr == pTransform)
		return E_FAIL;

	m_pModel = pModel;

	Safe_AddRef(m_pModel);

	m_pTransform = pTransform;

	Safe_AddRef(m_pTransform);

	return S_OK;
}

void CState_Idle::Enter()
{
	m_pModel->Set_AnimationIndex(m_pTransform, m_iAnimIndex, m_bLoop);
}

void CState_Idle::Update(_float fTimeDelta)
{
	m_pModel->Play_Animation(fTimeDelta);
}

void CState_Idle::Exit()
{

}


CState_Idle* CState_Idle::Create(CModel* pModel, CTransform* pTransform)
{
	CState_Idle* pInstance = new CState_Idle();

	if (FAILED(pInstance->Initilize(pModel, pTransform)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CState_Idle");
		return nullptr;
	}
	
	return pInstance;
}

void CState_Idle::Free()
{
	__super::Free();

	Safe_Release(m_pModel);
	Safe_Release(m_pTransform);
}