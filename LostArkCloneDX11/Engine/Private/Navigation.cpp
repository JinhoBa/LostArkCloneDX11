#include "Navigation.h"

#include "GameInstance.h"

#include "Cell.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	: CComponent{ Prototype },
	m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
	, m_pShaderCom{ Prototype.m_pShaderCom }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShaderCom);
#endif
}

_bool CNavigation::isMove(_fvector vPosition)
{
	_int iNeighborIndex = { -1 };

	_bool bInCell = m_Cells[m_iCurrentIndex]->isInCell(vPosition, &iNeighborIndex);

	if (true == bInCell)
		return true;
	else
	{
		if (-1 != iNeighborIndex)
		{
			m_iCurrentIndex = iNeighborIndex;
			return true;
		}
		
		return false;
	}
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
		if (true == in.eof())
			break;

		_float3 Points[CCell::POINT::POINT_END] = {};
		in.read(reinterpret_cast<_char*>(&Points), sizeof(_float3) * CCell::POINT::POINT_END);

		CCell* pCell = CCell::Create(m_pDevice, m_pContext,(_uint)m_Cells.size(), Points);

		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElement);

	if (nullptr == m_pShaderCom)
		return E_FAIL;
#endif // _DEBUG


	Set_NeighborIndices();

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

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if(FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	for (auto& pCell : m_Cells)
	{
		pCell->Render();
	}
	
	return S_OK;
}
#endif

void CNavigation::Set_NeighborIndices()
{
	for (auto& pSrcCell : m_Cells)
	{
		for (auto& pDstCell : m_Cells)
		{
			if (pSrcCell == pDstCell)
				continue;

			if (true == pDstCell->isNeighbor(pSrcCell->Get_Point(CCell::POINT::A), pSrcCell->Get_Point(CCell::POINT::B)))
				pDstCell->Set_Neighbor(CCell::LINE::AB, pSrcCell);

			if (true == pDstCell->isNeighbor(pSrcCell->Get_Point(CCell::POINT::B), pSrcCell->Get_Point(CCell::POINT::C)))
				pDstCell->Set_Neighbor(CCell::LINE::BC, pSrcCell);

			if (true == pDstCell->isNeighbor(pSrcCell->Get_Point(CCell::POINT::C), pSrcCell->Get_Point(CCell::POINT::A)))
				pDstCell->Set_Neighbor(CCell::LINE::CA, pSrcCell);
		}
	}
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

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShaderCom);
#endif
}