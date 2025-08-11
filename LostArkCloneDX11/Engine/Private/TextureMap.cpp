#include "TextureMap.h"

CTextureMap::CTextureMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CTextureMap::CTextureMap(CTextureMap& Prototype)
    : CComponent{ Prototype },
    m_iNumSRV{ Prototype.m_iNumSRV },
    m_SRVs{ Prototype.m_SRVs }
{
    for (auto pSRV : m_SRVs)
        Safe_AddRef(pSRV.second);
}

HRESULT CTextureMap::Initialize_Prototype(const _tchar* pFilePath, vector<_wstring>* pFileNames)
{
    m_iNumSRV = (_uint)(*pFileNames).size();

    _wstring strFilePath = {};

    for (_uint i = 0; i < m_iNumSRV; ++i)
    {
        _tchar szExtPath[MAX_PATH] = {};

        _wsplitpath_s((*pFileNames)[i].c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtPath, MAX_PATH);

        strFilePath = _wstring(pFilePath) + (*pFileNames)[i];

        ID3D11ShaderResourceView* pSRV = { nullptr };

        HRESULT hr = {};

        if (false == lstrcmp(szExtPath, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice, strFilePath.c_str(), nullptr, &pSRV);
        else if (false == lstrcmp(szExtPath, TEXT(".tga")))
            hr = E_FAIL;
        else
            hr = CreateWICTextureFromFile(m_pDevice, strFilePath.c_str(), nullptr, &pSRV);

        if (FAILED(hr))
            return E_FAIL;

        m_SRVs.emplace((*pFileNames)[i], pSRV);
    }

    return S_OK;
}

HRESULT CTextureMap::Initialize(void* pArg)
{
    return S_OK;
}

ID3D11ShaderResourceView* CTextureMap::Get_SRV(const _tchar* pFileName)
{
    auto iter = m_SRVs.find(_wstring(pFileName));

    if (iter == m_SRVs.end())
        return nullptr;

    return iter->second;
}

CTextureMap* CTextureMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFilePath, vector<_wstring>* pFileNames)
{
    CTextureMap* pInstance = new CTextureMap(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pFilePath, pFileNames)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTextureMap");
        return nullptr;
    }

    return pInstance;
}

CComponent* CTextureMap::Clone(void* pArg)
{
    CTextureMap* pInstance = new CTextureMap(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTextureMap");
        return nullptr;
    }

    return pInstance;
}

void CTextureMap::Free()
{
    __super::Free();

    for (auto pSRV : m_SRVs)
        Safe_Release(pSRV.second);
    m_SRVs.clear();
}
