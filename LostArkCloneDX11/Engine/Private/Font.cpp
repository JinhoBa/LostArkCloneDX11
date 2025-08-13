#include "Font.h"

CFont::CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }, m_pContext{pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont::Initialize(const _tchar* pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	if (nullptr == m_pFont)
		return E_FAIL;

	return S_OK;
}

HRESULT CFont::Draw_Font(SpriteBatch* pBatch)
{
	for (auto& Font : m_DrawFonts)
	{
		m_pFont->DrawString(pBatch, (Font->strWord).c_str(), XMLoadFloat4(&Font->vPositon), XMLoadFloat4(&Font->vColor), 0.f, g_XMZero, Font->fScale);
	}
	m_DrawFonts.clear();

	return S_OK;
}

void CFont::Add_FontDesc(FONT_DESC* pFontDesc)
{
	m_DrawFonts.push_back(pFontDesc);
}

void CFont::Clear()
{
	m_DrawFonts.clear();
}

CFont* CFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CFont* pInstance = new CFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFont");
		return nullptr;
	}
	return pInstance;
}

void CFont::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_DrawFonts.clear();

	Safe_Delete(m_pFont); 
}
