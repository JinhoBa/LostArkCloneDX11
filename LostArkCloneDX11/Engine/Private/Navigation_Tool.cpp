#ifdef _DEBUG
#include "Navigation_Tool.h"

#include "GameInstance.h"

#include "Cell.h"

CNavigation_Tool::CNavigation_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }, m_pContext{ pContext }, m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CNavigation_Tool::Initialize()
{
	
	m_iNumCells = 0;

	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElement);

	if (nullptr == m_pShaderCom)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

_bool CNavigation_Tool::Pick_Cell(_fvector vPickingPosition, _float3* pPoint)
{
	for (auto pCell : m_Cells)
	{
		if (true == pCell->Check_Points(vPickingPosition, pPoint))
			return true;
	}

	return false;
}

HRESULT CNavigation_Tool::Add_Sell(const _float3* pPoints)
{
	/* 시계방향 검사 */
	_vector vLinesAB = XMLoadFloat3(&pPoints[CCell::POINT::B]) - XMLoadFloat3(&pPoints[CCell::POINT::A]);
	_vector vLinesBC = XMLoadFloat3(&pPoints[CCell::POINT::C]) - XMLoadFloat3(&pPoints[CCell::POINT::B]);
	
	_float3 Points[CCell::POINT::POINT_END] = {};

	if (0.f > XMVectorGetY(XMVector3Cross(vLinesAB, vLinesBC))) // 이때 수정
	{
		memcpy(&Points[CCell::POINT::A], &pPoints[CCell::POINT::A], sizeof(_float3));
		memcpy(&Points[CCell::POINT::B], &pPoints[CCell::POINT::C], sizeof(_float3));
		memcpy(&Points[CCell::POINT::C], &pPoints[CCell::POINT::B], sizeof(_float3));
	}
	else
	{
		memcpy(&Points[CCell::POINT::A], &pPoints[CCell::POINT::A], sizeof(_float3));
		memcpy(&Points[CCell::POINT::B], &pPoints[CCell::POINT::B], sizeof(_float3));
		memcpy(&Points[CCell::POINT::C], &pPoints[CCell::POINT::C], sizeof(_float3));
	}

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, m_iNumCells, Points);

	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	++m_iNumCells;

	return S_OK;
}

void CNavigation_Tool::Remove_Sell()
{
	if(!m_Cells.empty())
	{
		Safe_Release(m_Cells.back());
		m_Cells.pop_back();
	}

}

HRESULT CNavigation_Tool::Save_File(const _char* pNavigationFilePath)
{
	SetUp_Neighbors();

	ofstream out(pNavigationFilePath, ios::binary);

	if (false == out.is_open())
	{
		MSG_BOX("Failed to Load Navigation Binanry File");
		return E_FAIL;
	}

	for (auto& pCell : m_Cells)
	{
		if (FAILED(pCell->Save_Binary(out)))
			return E_FAIL;
	}

	out.close();

	return S_OK;
}

HRESULT CNavigation_Tool::LoadFile(const _char* pNavigationFilePath)
{
	ifstream in(pNavigationFilePath, ios::binary);

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

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, (_uint)m_Cells.size(), Points);

		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	return S_OK;
}

HRESULT CNavigation_Tool::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	for (auto& pCell : m_Cells)
	{
		pCell->Render();
	}

	return S_OK;
}

void CNavigation_Tool::SetUp_Neighbors()
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

CNavigation_Tool* CNavigation_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavigation_Tool* pInstance = new CNavigation_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CNavigation_Tool");
		return nullptr;
	}

	return pInstance;
}

void CNavigation_Tool::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

	Safe_Release(m_pShaderCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

}

#endif