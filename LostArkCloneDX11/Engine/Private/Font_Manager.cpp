#include "Font_Manager.h"

#include "Font.h"

CFont_Manager::CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{ pDevice }, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont_Manager::Initialize()
{
	m_pBatch = new SpriteBatch(m_pContext);

	if (nullptr == m_pBatch)
		return E_FAIL;

	return S_OK;
}

void CFont_Manager::Draw_Fonts()
{
	m_pBatch->Begin();
	
	for (auto& pFonts : m_Fonts)
	{
		(pFonts.second)->Draw_Font(m_pBatch);
	}

	m_pBatch->End();
}

void CFont_Manager::Add_Font(const _wstring& strFontTag, CFont* pFont)
{
	m_Fonts.emplace(strFontTag, pFont);
}

void CFont_Manager::Add_FontDesc(const _wstring& strFontTag, FONT_DESC* pFontDesc)
{
	auto iter = m_Fonts.find(strFontTag);

	if (iter == m_Fonts.end())
		return;

	iter->second->Add_FontDesc(pFontDesc);
}
void CFont_Manager::Clear()
{
	for (auto Font : m_Fonts)
	{
		Font.second->Clear();
	}
}

CFont_Manager* CFont_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFont_Manager* pInstance = new CFont_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CFont_Manager");
		return nullptr;
	}

	return pInstance;
}

void CFont_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto pFont : m_Fonts)
		Safe_Release(pFont.second);
	m_Fonts.clear();

	Safe_Delete(m_pBatch);
}
