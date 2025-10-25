#include "Shadow.h"

#include "Shader.h"

CShadow::CShadow()
{
}

HRESULT CShadow::Ready_Shadow_Light(const SHADOW_LIGHT_DESC& Desc)
{
    XMStoreFloat4x4(&m_TransformationMatrices[ENUM_TO_INT(D3DTS::VIEW)],
        XMMatrixLookAtLH(XMLoadFloat4(&Desc.vEye), XMLoadFloat4(&Desc.vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

    XMStoreFloat4x4(&m_TransformationMatrices[ENUM_TO_INT(D3DTS::PROJ)],
        XMMatrixPerspectiveFovLH(Desc.fFovy, Desc.fAspect, Desc.fNear, Desc.fFar));

    return S_OK;
}

HRESULT CShadow::Bind_Shadow_Resource(CShader* pShader, const _char* pContantName, D3DTS eType) const
{
    return pShader->Bind_Matrix(pContantName, &m_TransformationMatrices[ENUM_TO_INT(eType)]);
}

CShadow* CShadow::Create()
{
    return new CShadow();
}

void CShadow::Free()
{
    __super::Free();
}
