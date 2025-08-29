#pragma once

#include "Base.h"

/* 뼈 */
/* aiBone : 어떤 정점에게 얼마나? */
/* aiNode : 계층구조를 표현 + 상태행렬을 표현 */
/* aiAnimNode : */

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}
	_bool Compare_Name(const _char* pBoneName) const {
		return !strcmp(pBoneName, m_szName);
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

private:
	_char				m_szName[MAX_PATH] = {};
	_float4x4			m_TransformationMatrix = {}; /* 이 뼈만의 상태변환행렬 */
	_float4x4			m_CombinedTransformationMatrix = {}; /* m_TransformatinoMatrix * Parent`s m_CombinedTransformationMatrix */
	_int				m_iParentBoneIndex = { -1 };
	/*CBone*				m_pParent = { nullptr };*/

public:
	static CBone* Create(const aiNode* pAINode, _int iParentIndex);
	virtual void Free() override;
};

NS_END