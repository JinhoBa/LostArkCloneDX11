#include "Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{pDevice, pContext}
{
}

CTransform::CTransform(CTransform& Prototype)
    : CComponent{Prototype}
{
}

HRESULT CTransform::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    return S_OK;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTransform* pInstance = new CTransform(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CTransform");
        return nullptr;
    }

    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CComponent* pInstance = new CTransform(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CTransform");
        return nullptr;
    }

    return pInstance;
}

void CTransform::Free()
{
}
