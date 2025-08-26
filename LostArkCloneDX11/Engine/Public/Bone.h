#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT Initialize(aiNode* pNode, _int iParentIndex);
	void	Update_CombinedTransformationMatrix();

private:
	_int		m_iParentIndex = {};
	_char		m_szBoneName[MAX_PATH] = {};

	_float4x4	m_TransformationMatrix = {};
	_float4x4	m_CombinedTransformationMatrix = {};

public:
	static CBone* Create(aiNode* pNode, _int iParentIndex);
	virtual void Free() override;
};

NS_END
