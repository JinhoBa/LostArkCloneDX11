#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CChannel : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const class CModel* pModel, const aiNodeAnim* pAiChannels);
	HRESULT Initialize(ifstream& in);
	void Update_TransformationMatrix(const vector<class CBone*> Bones, _float fCurrentTrackPosition);
	void Reset_KeyFrame() { m_iCurKeyFrameIndex = 0; }

#ifdef _DEBUG
	void Save_To_Binary(ofstream& out);
#endif

private:
	_char				m_szName[MAX_NAME] = {};

	_uint				m_iNumKeyFrames = {};
	_uint				m_iBoneIndex = {};
	_uint				m_iCurKeyFrameIndex = {0};
	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(const class CModel* pModel, const aiNodeAnim* pAiChannels);
	static CChannel* Create(ifstream& in);
	virtual void Free() override;
};

NS_END