#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{pDevice, pContext}
{
}

CVIBuffer::CVIBuffer(CVIBuffer& Prototype)
    : CComponent{Prototype},
    m_pVB{Prototype.m_pVB},
    m_pIB{Prototype.m_pIB},
    m_iNumVertices{Prototype.m_iNumVertices},
    m_iVertexStride{Prototype.m_iVertexStride},
    m_iNumIndices{Prototype.m_iNumIndices},
    m_iIndexStride{Prototype.m_iIndexStride},
    m_eIndexFormat{Prototype.m_eIndexFormat},
    m_iNumVertexBuffers{Prototype.m_iNumVertexBuffers},
    m_ePrimitive{Prototype.m_ePrimitive}
{
    Safe_AddRef(m_pVB);
    Safe_AddRef(m_pIB);

}

HRESULT CVIBuffer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer::Bind_Resources(ID3D11InputLayout* pLayout)
{
    if (nullptr == pLayout)
        return E_FAIL;

    ID3D11Buffer* VertexBuffers[] = {
        m_pVB,
    };

    _uint VertexStrides[] = {
        m_iVertexStride,
    };

    _uint Offset[] = {
        0,
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, VertexBuffers, VertexStrides, Offset);
    m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_ePrimitive);

    m_pContext->IASetInputLayout(pLayout);

    return S_OK;
}

HRESULT CVIBuffer::Render()
{
    m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

    return S_OK;
}

void CVIBuffer::Free()
{
    __super::Free();

    if (false == m_isCloned)
        Safe_Delete_Array(m_pVertexPositions);

    Safe_Release(m_pVB);
    Safe_Release(m_pIB);
}
