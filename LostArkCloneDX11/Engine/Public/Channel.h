#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CChannel : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	KEYFRAME& Get_LastKeyFrames() { return m_KeyFrames.back(); }

public:
	HRESULT Initialize(const class CModel* pModel, const aiNodeAnim* pAiChannels);
	HRESULT Initialize(ifstream& in);
	void Update_TransformationMatrix(_uint* pCurKeyFrameIndex, const vector<class CBone*> Bones, _float fCurrentTrackPosition);
	void Update_TransformationMatrix(_uint* pCurKeyFrameIndex, const vector<class CBone*> Bones, _float fRatio, vector<KEYFRAME>& PreAnimKeyFrames);

#ifdef _DEBUG
	void Save_To_Binary(ofstream& out);
#endif

private:
	_char				m_szName[MAX_NAME] = {};

	_uint				m_iNumKeyFrames = {};
	_uint				m_iBoneIndex = {};

	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(const class CModel* pModel, const aiNodeAnim* pAiChannels);
	static CChannel* Create(ifstream& in);
	virtual void Free() override;
};

NS_END