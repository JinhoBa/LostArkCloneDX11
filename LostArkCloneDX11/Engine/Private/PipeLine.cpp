#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Update_PipeLine(PIPELINE ePipLine, _float4x4& Matrix)
{
	XMStoreFloat4x4(&m_PipLineMatrix[ENUM_TO_INT(ePipLine)], XMLoadFloat4x4(&Matrix));
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine();
}

void CPipeLine::Free()
{
	__super::Free();
}
