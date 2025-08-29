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
	void	Update_TransformationMatrix(const vector<class CBone*> Bones, _float fTimeDelta);
private:
	_uint					m_iNumChannels = {};
	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};

	vector<class CChannel*>	m_Channels;

public:
	static CAnimation* Create(const class CModel* pModel, const aiAnimation* pAiAnimation);
	virtual void Free() override;
};

NS_END