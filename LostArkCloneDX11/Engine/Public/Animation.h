#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const class CModel* pModel, const aiAnimation* pAiAnimation);
	HRESULT Initialize(ifstream& in);
	void	Update_TransformationMatrix(const vector<class CBone*> Bones, _float fTimeDelta);
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

public:
	static CAnimation* Create(const class CModel* pModel, const aiAnimation* pAiAnimation);
	static CAnimation* Create(ifstream& in);
	virtual void Free() override;
};

NS_END