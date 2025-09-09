#pragma once
#include "Client_Defines.h"
#include "HUD.h"

NS_BEGIN(Client)

class CBuffUI : public CHUD
{
private:
	CBuffUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuffUI(const CBuffUI& Prototype);
	virtual ~CBuffUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_uint				m_iNumBuff = {};
	_float2				m_DefaultFontPosition = {};
	_float				m_fDefaultSize = {};

	list<_uint>			m_BuffIDs;
	list<_float>		m_BuffCoolTimes;

	class CPlayer*		m_pPlayer = { nullptr };
	CTexture*			m_pFrameTextureCom = { nullptr };

	FONT_DESC			m_CoolTimeFont = {};

private:
	HRESULT Add_Components();
	HRESULT Ready_Font();


public:
	static CBuffUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
