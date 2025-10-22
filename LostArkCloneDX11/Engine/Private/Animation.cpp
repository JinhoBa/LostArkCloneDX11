#include "Animation.h"

#include "Channel.h"
#include "Bone.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype)
	:m_iNumChannels{Prototype.m_iNumChannels}
	,m_fDuration{Prototype.m_fDuration}
	,m_fTickPerSecond{Prototype.m_fTickPerSecond}
	, m_fCurrentTrackPosition{Prototype.m_fCurrentTrackPosition}
	, m_Channels{Prototype.m_Channels}
	, m_iCurKeyFrameIndices{Prototype.m_iCurKeyFrameIndices}
{
	memcpy(&m_szName, Prototype.m_szName, MAX_NAME);

	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
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
		m_iCurKeyFrameIndices.push_back(0);
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
	m_iCurKeyFrameIndices.reserve((size_t)m_iNumChannels);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(in);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
		m_iCurKeyFrameIndices.push_back(0);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrix(const vector<CBone*> Bones, _float fTimeDelta)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	_uint iIndex = {};

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(&m_iCurKeyFrameIndices[iIndex++], Bones, m_fCurrentTrackPosition);
	}
}

void CAnimation::Update_TransformationMatrix(const vector<class CBone*> Bones, _float fRatio, vector<KEYFRAME>& PreAnimKeyFrames)
{
	_uint iIndex = {};
	
	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(
			&m_iCurKeyFrameIndices[iIndex], Bones, fRatio, PreAnimKeyFrames);
		++iIndex;
	}
}

void CAnimation::Reset_TrackPosition()
{
	m_fCurrentTrackPosition = 0;

	for (auto& iIndex : m_iCurKeyFrameIndices)
		iIndex = 0;
}

void CAnimation::Update_Debug_TransformationMatrix(const vector<class CBone*> Bones, _float fTrackPosition)
{
	Reset_TrackPosition();

	m_fCurrentTrackPosition = max(0.f, fTrackPosition);

	_uint iIndex = {};

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(&m_iCurKeyFrameIndices[iIndex++], Bones, m_fCurrentTrackPosition);
	}
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

CAnimation* CAnimation::Clone()
{
	return  new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
