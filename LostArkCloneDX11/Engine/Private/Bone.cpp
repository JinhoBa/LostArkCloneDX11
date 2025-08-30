#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentIndex)
{
	strcpy_s(m_szName, pAINode->mName.data);

	m_iParentBoneIndex = iParentIndex;

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
		 
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CBone::Initialize(ifstream& in)
{
	in.read(reinterpret_cast<_char*>(m_szName), MAX_NAME);

	in.read(reinterpret_cast<_char*>(&m_iParentBoneIndex), sizeof(_int));

	in.read(reinterpret_cast<_char*>(&m_TransformationMatrix), sizeof(_float4x4));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	if (-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, 
			XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * Bones[m_iParentBoneIndex]->Get_CombinedTransformationMatrix());
}

void CBone::Save_To_Binary(ofstream& out, _fmatrix PreTransformMatrix)
{
	out.write(reinterpret_cast<const _char*>(m_szName), MAX_NAME);
	out.write(reinterpret_cast<const _char*>(&m_iParentBoneIndex), sizeof(_uint));

	// PreTransformMatrix와 Transpose 변환한 상태로 저장

	if (-1 == m_iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_TransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
	}

	out.write(reinterpret_cast<const _char*>(&m_TransformationMatrix), sizeof(_float4x4));

}

CBone* CBone::Create(const aiNode* pAINode, _int iParentIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Create(ifstream& in)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(in)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();
}
