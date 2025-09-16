#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	vector<class CChannel*>& Get_Channels() { return m_Channels; }
	_bool IsAnimationFinished() { return m_fCurrentTrackPosition > m_fDuration; }
	_bool IsAnimationFinished(_float fOffSet) { return m_fCurrentTrackPosition > (m_fDuration * fOffSet); }
	_char* GetAnimationName() { return m_szName; }
	_float Get_TrackPosition() const { return m_fCurrentTrackPosition; }


public:
	HRESULT Initialize(const class CModel* pModel, const aiAnimation* pAiAnimation);
	HRESULT Initialize(ifstream& in);
	void	Update_TransformationMatrix(const vector<class CBone*> Bones, _float fTimeDelta);
	void	Update_TransformationMatrix(const vector<class CBone*> Bones, _float fRatio, vector<KEYFRAME>& PreAnimKeyFrames);
	void    Reset_TrackPosition();

#ifdef _DEBUG
	void Save_To_Binary(ofstream& out);
#endif

private:
	_char					m_szName[MAX_NAME] = {};
	_uint					m_iNumChannels = {};
	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};

	vector<class CChannel*>	m_Channels;
	vector<_uint>			m_iCurKeyFrameIndices;




public:
	static CAnimation* Create(const class CModel* pModel, const aiAnimation* pAiAnimation);
	static CAnimation* Create(ifstream& in);
	CAnimation* Clone();
	virtual void Free() override;
};

NS_END