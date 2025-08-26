#include "Bone.h"


CBone::CBone()
{

}

HRESULT CBone::Initialize(aiNode* pNode, _int iParentIndex)
{
	m_iParentIndex = iParentIndex;

	strcpy_s(m_szBoneName, pNode->mName.data);

	memcpy(&m_TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());


	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix()
{
	
}

CBone* CBone::Create(aiNode* pNode, _int iParentIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pNode, iParentIndex)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBone");
		return nullptr;
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();
}
