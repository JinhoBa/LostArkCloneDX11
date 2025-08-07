#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice, pContext}
{
}

CCamera::CCamera(const CCamera& Prototype)
    : CGameObject{Prototype}
{
}

HRESULT CCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    _float2 vViewPort = m_pGameInstance->Get_WinSize();

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;
    m_fFovy = pDesc->fFovy;
    m_fAspect = vViewPort.x / vViewPort.y;

    m_fWinSizeX = vViewPort.x;
    m_fWinSizeY = vViewPort.y;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vEye), 1.f));
    m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&pDesc->vLookAt), 1.f));

    return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
   
}

void CCamera::Update(_float fTimeDelta)
{

}

void CCamera::Late_Update(_float fTimeDelta)
{
}

void CCamera::Bind_Transform()
{
    m_pGameInstance->Set_Transform(D3DTS::VIEW, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())));
    m_pGameInstance->Set_Transform(D3DTS::PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
}

void CCamera::Free()
{
    __super::Free();
}
