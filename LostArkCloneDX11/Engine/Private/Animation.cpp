#include "Animation.h"

#include "Channel.h"
#include "Bone.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const class CModel* pModel, const aiAnimation* pAiAnimation)
{
	strcpy_s(m_szName, pAiAnimation->mName.data);
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

HRESULT CAnimation::Initialize(ifstream& in)
{
	in.read(reinterpret_cast<_char*>(m_szName), MAX_NAME);

	in.read(reinterpret_cast<_char*>(&m_iNumChannels), sizeof(_uint));

	in.read(reinterpret_cast<_char*>(&m_fDuration), sizeof(_float));
	in.read(reinterpret_cast<_char*>(&m_iNumChannels), sizeof(_uint));
	in.read(reinterpret_cast<_char*>(&m_fTickPerSecond), sizeof(_float));

	m_Channels.reserve((size_t)m_iNumChannels);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(in);

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

void CAnimation::Reset_TrackPosition()
{
	m_fCurrentTrackPosition = 0;

	for (auto& pChannel : m_Channels)
		pChannel->Reset_KeyFrame();
}

void CAnimation::Save_To_Binary(ofstream& out)
{
	out.write(reinterpret_cast<const _char*>(m_szName), MAX_NAME);

	out.write(reinterpret_cast<const _char*>(&m_iNumChannels), sizeof(_uint));

	out.write(reinterpret_cast<const _char*>(&m_fDuration), sizeof(_float));
	out.write(reinterpret_cast<const _char*>(&m_iNumChannels), sizeof(_uint));
	out.write(reinterpret_cast<const _char*>(&m_fTickPerSecond), sizeof(_float));

	for (auto& pChannel : m_Channels)
	{
		pChannel->Save_To_Binary(out);
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

CAnimation* CAnimation::Create(ifstream& in)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(in)))
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
