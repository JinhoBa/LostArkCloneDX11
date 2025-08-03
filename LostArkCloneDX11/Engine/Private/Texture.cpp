#include "Texture.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{pDevice, pContext}
{
}

CTexture::CTexture(CTexture& Prototype)
    : CComponent{Prototype},
    m_iNumSRV{Prototype.m_iNumSRV},
    m_pSRVs{Prototype.m_pSRVs}
{
    for (auto pSRV : m_pSRVs)
        Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTexture)
{
    m_iNumSRV = iNumTexture;

    _tchar szExtPath[MAX_PATH] = {};

    _wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExtPath, MAX_PATH);

    _tchar szFilePath[MAX_PATH] = {};

    for (_uint i = 0; i < m_iNumSRV; ++i)
    {
        ID3D11ShaderResourceView* pSRV = { nullptr };

        HRESULT hr = {};

        wsprintf(szFilePath, pTextureFilePath, i);

        if (false == lstrcmp(szExtPath, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);
        else if (false == lstrcmp(szExtPath, TEXT(".tga")))
            hr = E_FAIL;
        else
            hr = CreateWICTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);

        if (FAILED(hr))
            return E_FAIL;

        m_pSRVs.push_back(pSRV);
    }

    return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
    return S_OK;
}

ID3D11ShaderResourceView* CTexture::Get_SRV(_uint iIndex)
{
    if (m_iNumSRV <= iIndex)
        return nullptr;

    return m_pSRVs[iIndex];
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, _uint iNumTexture)
{
    CTexture* pInstance = new CTexture(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pFilePath, iNumTexture)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTexture");
        return nullptr;
    }

    return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
    CTexture* pInstance = new CTexture(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTexture");
        return nullptr;
    }

    return pInstance;
}

void CTexture::Free()
{
    __super::Free();

    for (auto pSRV : m_pSRVs)
        Safe_Release(pSRV);
    m_pSRVs.clear();
}
