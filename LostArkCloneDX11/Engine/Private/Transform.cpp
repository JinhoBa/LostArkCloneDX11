#include "Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{pDevice, pContext}
{
}

CTransform::CTransform(CTransform& Prototype)
    : CComponent{Prototype}
{
}

_float3 CTransform::Get_Scale() const
{
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK)))
    );
}

void CTransform::Set_Scale(_float3 vScale)
{
    _vector vRight = XMVector3Normalize(Get_State(STATE::RIGHT)) * vScale.x;
    _vector vUp = XMVector3Normalize(Get_State(STATE::UP)) * vScale.y;
    _vector vLook = XMVector3Normalize(Get_State(STATE::LOOK)) * vScale.z;

    Set_State(STATE::RIGHT, vRight);
    Set_State(STATE::UP, vUp);
    Set_State(STATE::LOOK, vLook);
}

_float4x4& CTransform::Get_WorldMatrixInv()
{
    XMStoreFloat4x4(&m_WorldMatrixInv, (XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix))));
    return m_WorldMatrixInv;
}

HRESULT CTransform::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

    if (nullptr == pArg)
    {
        m_fSpeedPersec = 1.f;
        m_fRotatePersec = 1.f;
        return S_OK;
    }

    TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

    m_fSpeedPersec = pDesc->fSpeedPersec;
    m_fRotatePersec = pDesc->fRotatePersec;

    return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
    _vector vPosition = Get_State(STATE::POSITION);

    vPosition += XMVector3Normalize(Get_State(STATE::LOOK)) * m_fSpeedPersec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
    _vector vPosition = Get_State(STATE::POSITION);

    vPosition += XMVector3Normalize(Get_State(STATE::LOOK)) * m_fSpeedPersec * -fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
    _vector vPosition = Get_State(STATE::POSITION);

    vPosition += XMVector3Normalize(Get_State(STATE::RIGHT)) * m_fSpeedPersec * -fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
    _vector vPosition = Get_State(STATE::POSITION);

    vPosition += XMVector3Normalize(Get_State(STATE::RIGHT)) * m_fSpeedPersec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::MoveTo(_float fTimeDelta, _fvector vTargetPos)
{
    _vector vPosition = Get_State(STATE::POSITION);
    _vector vDirection = vTargetPos - vPosition;

    if (0.1f >= XMVectorGetX(XMVector3Length(vDirection)))
        return;

    vPosition += XMVector3Normalize(vDirection) * m_fSpeedPersec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
    _vector vRight = Get_State(STATE::RIGHT);
    _vector vUp = Get_State(STATE::UP);
    _vector vLook = Get_State(STATE::LOOK);

    _matrix matRotation = XMMatrixRotationAxis(vAxis, m_fRotatePersec * fTimeDelta);

    Set_State(STATE::RIGHT, (XMVector3TransformNormal(vRight, matRotation)));
    Set_State(STATE::UP, (XMVector3TransformNormal(vUp, matRotation)));
    Set_State(STATE::LOOK, (XMVector3TransformNormal(vLook, matRotation)));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
    _float3 vScale = Get_Scale();

    _vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    _vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

    _matrix matRotation = XMMatrixRotationAxis(vAxis, fRadian);

    Set_State(STATE::RIGHT, (XMVector3TransformNormal(vRight, matRotation)) * vScale.x);
    Set_State(STATE::UP, (XMVector3TransformNormal(vUp, matRotation)) * vScale.y);
    Set_State(STATE::LOOK, (XMVector3TransformNormal(vLook, matRotation)) * vScale.z);
}

void CTransform::Rotation(_float fRadianX, _float fRadianY, _float fRadianZ)
{
    _float3 vScale = Get_Scale();

    _vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    _vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

    _matrix matRotation = XMMatrixRotationRollPitchYaw(fRadianX, fRadianY, fRadianZ);

    Set_State(STATE::RIGHT, (XMVector3TransformNormal(vRight, matRotation)) * vScale.x);
    Set_State(STATE::UP, (XMVector3TransformNormal(vUp, matRotation)) * vScale.y);
    Set_State(STATE::LOOK, (XMVector3TransformNormal(vLook, matRotation)) * vScale.z);
}

void CTransform::LookAt(_fvector vTarget)
{
    _float3 vScale = Get_Scale();

    _vector vLook = vTarget - Get_State(STATE::POSITION);

    vLook = XMVector3Normalize(vLook);

    _vector vUpDir= XMVectorSet(0.f, 1.f, 0.f, 0.f);

    _vector vRight = XMVector3Normalize(XMVector3Cross(vUpDir, vLook));

    _vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

    Set_State(STATE::RIGHT, (vRight * vScale.x));
    Set_State(STATE::UP, (vUp * vScale.y));
    Set_State(STATE::LOOK, (vLook * vScale.z));

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
    __super::Free();
}
