#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CFont final : public CBase
{
private:
	CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);

public:
	HRESULT		Draw_Font(SpriteBatch* pBatch);
	void		Add_FontDesc(FONT_DESC* pFontDesc);
	void		Clear();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	
	SpriteFont*				m_pFont = {nullptr};
	list<FONT_DESC*>		m_DrawFonts = {};

public:
	static CFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

NS_END