#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CTexture;
class ENGINE_DLL CUIAnimation final: public CComponent
{
public:
	typedef struct UIAnim_Desc
	{
		_bool	bLoop{};
		_uint	iStartFrame{}; // 시작 프레임
		_uint	iEndFrame{}; // 끝 프레임 수
		_float	fAnimTime{}; // 애니메이션 재생 시간

		CTexture* pTextureCom{};
	}UIANIM_DESC;

private:
	CUIAnimation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIAnimation(CUIAnimation& Prototype);
	virtual ~CUIAnimation() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Update(_float fTimeDelta);
	void Draw();

public:
	_bool	isEndFrame() const { return m_iCurFrame == m_iEndFrame; };

private:
	_bool		m_bLoop = {};

	_uint		m_iStartFrame = {};
	_uint		m_iEndFrame = {};
	_uint		m_iCurFrame = {};

	_float		m_fAnimTime = {};

	_float		m_fFrameTime = {};
	_float		m_fTimeAcc = {};

	CTexture*	m_pTextureCom = { nullptr };

public:
	static CUIAnimation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END