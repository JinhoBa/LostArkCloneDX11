#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	_vector Get_Position();
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	const _float4x4* Get_CombinedTransformationMatrixPrt() const {
		return &m_CombinedTransformationMatrix;
	}
	_matrix Get_TransformationMatrix() const {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}
	_bool Compare_Name(const _char* pBoneName) const {
		return !strcmp(pBoneName, m_szName);
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentIndex);
	HRESULT Initialize(ifstream& in);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

	void Save_To_Binary(ofstream& out, _fmatrix PreTransformMatrix);

private:
	_char				m_szName[MAX_NAME] = {};
	_float4x4			m_TransformationMatrix = {}; 
	_float4x4			m_CombinedTransformationMatrix = {};
	_int				m_iParentBoneIndex = { -1 };


public:
	static CBone* Create(const aiNode* pAINode, _int iParentIndex);
	static CBone* Create(ifstream& in);
	CBone* Clone();
	virtual void Free() override;
};

NS_END