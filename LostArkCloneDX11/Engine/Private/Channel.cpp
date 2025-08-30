#include "Channel.h"

#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const CModel* pModel, const aiNodeAnim* pAiChannels)
{
	strcpy_s(m_szName, pAiChannels->mNodeName.data);
	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);

	if (-1 == m_iBoneIndex)
		return E_FAIL;

	_float3 vScale = {};
	_float4 vRotation = {};
	_float3 vTranslation = {};

	m_iNumKeyFrames = max(pAiChannels->mNumScalingKeys, pAiChannels->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAiChannels->mNumPositionKeys);

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};

		if (i < pAiChannels->mNumScalingKeys)
		{
			memcpy(&vScale, &pAiChannels->mScalingKeys[i].mValue, sizeof(_float3));

			KeyFrame.fTrackPosition = (_float)pAiChannels->mScalingKeys[i].mTime;
		}

		if (i < pAiChannels->mNumRotationKeys)
		{
			vRotation.x = pAiChannels->mRotationKeys[i].mValue.x;
			vRotation.y = pAiChannels->mRotationKeys[i].mValue.y;
			vRotation.z = pAiChannels->mRotationKeys[i].mValue.z;
			vRotation.w = pAiChannels->mRotationKeys[i].mValue.w;

			KeyFrame.fTrackPosition = (_float)pAiChannels->mRotationKeys[i].mTime;
		}

		if (i < pAiChannels->mNumPositionKeys)
		{
			memcpy(&vTranslation, &pAiChannels->mPositionKeys[i].mValue, sizeof(_float3));

			KeyFrame.fTrackPosition = (_float)pAiChannels->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vTranslation = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}


	return S_OK;
}

HRESULT CChannel::Initialize(ifstream& in)
{
	in.read(reinterpret_cast<_char*>(m_szName), MAX_NAME);
	in.read(reinterpret_cast<_char*>(&m_iBoneIndex), sizeof(_uint));
	in.read(reinterpret_cast<_char*>(&m_iNumKeyFrames), sizeof(_uint));
	
	if (-1 == m_iBoneIndex)
		return E_FAIL;

	_float3 vScale = {};
	_float4 vRotation = {};
	_float3 vTranslation = {};

	m_KeyFrames.reserve((size_t)m_iNumKeyFrames);

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};

		in.read(reinterpret_cast<_char*>(&KeyFrame), sizeof(KEYFRAME));

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrix(const vector<CBone*> Bones, _float fCurrentTrackPosition)
{
	KEYFRAME LastKeyFrame = m_KeyFrames.back();

	_vector vScale = {};
	_vector vRotation = {};
	_vector vTranslation = {};

	if (LastKeyFrame.fTrackPosition <= fCurrentTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else 	/* linear interpolation state */
	{
		if (fCurrentTrackPosition >= m_KeyFrames[m_iCurKeyFrameIndex + 1].fTrackPosition)
			++m_iCurKeyFrameIndex;

		_float fRatio = (fCurrentTrackPosition - m_KeyFrames[m_iCurKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[m_iCurKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[m_iCurKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(
			XMLoadFloat3(&m_KeyFrames[m_iCurKeyFrameIndex].vScale),
			XMLoadFloat3(&m_KeyFrames[m_iCurKeyFrameIndex + 1].vScale),
			fRatio);

		vRotation = XMQuaternionNormalize(XMQuaternionSlerp(
			XMLoadFloat4(&m_KeyFrames[m_iCurKeyFrameIndex].vRotation), 
			XMLoadFloat4(&m_KeyFrames[m_iCurKeyFrameIndex + 1].vRotation),
			fRatio));

		vTranslation = XMVectorLerp(
			XMVectorSetW(XMLoadFloat3(&m_KeyFrames[m_iCurKeyFrameIndex].vTranslation),1.f ), 
			XMVectorSetW(XMLoadFloat3(&m_KeyFrames[m_iCurKeyFrameIndex + 1].vTranslation), 1.f), 
			fRatio);
	}

	_matrix BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	
	Bones[m_iBoneIndex]->Set_TransformationMatrix(BoneTransformationMatrix);
}

void CChannel::Save_To_Binary(ofstream& out)
{
	out.write(reinterpret_cast<const _char*>(m_szName), MAX_NAME);

	out.write(reinterpret_cast<const _char*>(&m_iBoneIndex), sizeof(_int));
	out.write(reinterpret_cast<const _char*>(&m_iNumKeyFrames), sizeof(_uint));

	for (auto& Keyframe : m_KeyFrames)
	{
		out.write(reinterpret_cast<const _char*>(&Keyframe), sizeof(KEYFRAME));
	}
}

CChannel* CChannel::Create(const CModel* pModel, const aiNodeAnim* pAiChannels)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pModel, pAiChannels)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CChannel");
		return nullptr;
	}

	return pInstance;
}

CChannel* CChannel::Create(ifstream& in)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(in)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CChannel");
		return nullptr;
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();

	m_KeyFrames.clear();
}
