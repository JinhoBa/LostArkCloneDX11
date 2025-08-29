#include "Animation.h"

#include "Channel.h"
#include "Bone.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const class CModel* pModel, const aiAnimation* pAiAnimation)
{
	m_fDuration = (_float)pAiAnimation->mDuration;
	m_iNumChannels = pAiAnimation->mNumChannels;
	m_fTickPerSecond = (_float)pAiAnimation->mTicksPerSecond;

	for (_uint i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pModel, pAiAnimation->mChannels[i]);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrix(const vector<CBone*> Bones, _float fTimeDelta)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition);
	}
}

CAnimation* CAnimation::Create(const class CModel* pModel, const aiAnimation* pAiAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pModel, pAiAnimation)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CAnimation");
		return nullptr;
	}

	return pInstance;
}

void CAnimation::Free()
{
	__super::Free();

	for (auto pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
