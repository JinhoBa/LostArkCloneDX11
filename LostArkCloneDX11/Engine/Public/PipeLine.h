#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CPipeLine final : public CBase
{
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	const _float4x4& Get_PipeLine(PIPELINE ePipLine) const { return m_PipLineMatrix[ENUM_TO_INT(ePipLine)]; }

public:
	void Update_PipeLine(PIPELINE ePipLine, _float4x4& Matrix);

private:
	_float4x4	m_PipLineMatrix[ENUM_TO_INT(PIPELINE::END)] = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

NS_END