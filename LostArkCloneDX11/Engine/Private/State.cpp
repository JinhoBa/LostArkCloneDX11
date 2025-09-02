#include "State.h"

CState::CState()
{
}

void CState::Set_State(_uint iIndex, _bool bLoop)
{
	m_iAnimIndex = iIndex;
	m_bLoop = bLoop;
}

HRESULT CState::Initilize(CModel* pModel, CTransform* pTransform)
{

	return S_OK;
}

void CState::Enter()
{

}

void CState::Update(_float fTimeDelta)
{

}

void CState::Exit()
{

}


void CState::Free()
{
	__super::Free();
}