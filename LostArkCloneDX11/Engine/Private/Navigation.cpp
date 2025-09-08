#include "Navigation.h"

#include "Cell.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CComponent& Prototype)
	: CComponent{ Prototype }
{
}
HRESULT CNavigation::Initialize_Prototype(const _char* pNavigaitonFilePath)
{
	ifstream in(pNavigaitonFilePath, ios::binary);

	if (false == in.is_open())
	{
		MSG_BOX("Failed to Load Navigation Binanry File");
		return E_FAIL;
	}

	while (true)
	{
		_float3 Points[CCell::POINT::END] = {};
		in.read(reinterpret_cast<_char*>(&Points), sizeof(_float3) * CCell::POINT::END);

		if (true == in.eof())
			break;

		CCell* pCell = CCell::Create((_uint)m_Cells.size(), Points);

		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}


	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	m_iCurrentIndex = pDesc->iCurrentIndex;

	return S_OK;
}

void CNavigation::Upadte(_float fTimeDelta)
{
	
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pNavigaitonFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigaitonFilePath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CNavigation");
		return nullptr;
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CComponent* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CNavigation");
		return nullptr;
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();
}