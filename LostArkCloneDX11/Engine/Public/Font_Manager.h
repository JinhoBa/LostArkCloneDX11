#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CFont;
class CFont_Manager : public CBase
{
private:
	CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFont_Manager() = default;

public:
	HRESULT		Initialize();
	void		Draw_Fonts();
	void		Add_Font(const _wstring& strFontTag, CFont* pFont);
	void		Add_FontDesc(const _wstring& strFontTag, FONT_DESC* pFontDesc);
	void		Clear();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	SpriteBatch*			m_pBatch = { nullptr };

	map<_wstring, CFont*>	m_Fonts = {};
	

public:
	static CFont_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END