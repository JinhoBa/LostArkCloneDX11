#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice, pContext}
{
}

CCamera::CCamera(CCamera& Prototype)
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

    D3D11_VIEWPORT ViewPort = {};
    _uint iNumViewPort = { 1 };

    m_pContext->RSGetViewports(&iNumViewPort, &ViewPort);

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;
    m_fFovy = pDesc->fFovy;
    m_fAspect = ViewPort.Width / ViewPort.Height;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;



    return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Update_PipeLine(PIPELINE::VIEW, m_pTransformCom->Get_WorldMatrixInv());
    m_pGameInstance->Update_PipeLine(PIPELINE::VIEWINV, m_pTransformCom->Get_WorldMatrix());

    _matrix Proj = XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar);

    _float4x4 ProjMatrix{}, ProjMatrixInv{};

    XMStoreFloat4x4(&ProjMatrix, Proj);
    XMStoreFloat4x4(&ProjMatrixInv, XMMatrixInverse(nullptr, Proj));

    m_pGameInstance->Update_PipeLine(PIPELINE::PROJ, ProjMatrix);
    m_pGameInstance->Update_PipeLine(PIPELINE::PROJINV, ProjMatrixInv);
}

void CCamera::Update(_float fTimeDelta)
{

}

void CCamera::Late_Update(_float fTimeDelta)
{
}

void CCamera::Free()
{
    __super::Free();
}
