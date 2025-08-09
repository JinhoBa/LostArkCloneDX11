#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{pDevice}, m_pContext{pContext},
    m_pGameInstance{CGameInstance::GetInstance()}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

  
    _float2 vWinSize = m_pGameInstance->Get_WinSize();

    m_fWinSizeX = vWinSize.x;
    m_fWinSizeY = vWinSize.y;

    return S_OK;
}

void CPicking::Update()
{
    POINT ptMouse = {};

    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);

    _float4 vMousePosition = {};

    vMousePosition.x = ptMouse.x / (m_fWinSizeX * 0.5f) - 1.f;
    vMousePosition.y = ptMouse.y / -(m_fWinSizeY * 0.5f) + 1.f;
    vMousePosition.z = 0.f;
    vMousePosition.w = 1.f;

    _matrix ProjMatrixInv = m_pGameInstance->Get_Transfrom_MatrixInverse(D3DTS::PROJ);
    
    _vector vViewMousePos = XMVector4Transform(XMLoadFloat4(&vMousePosition), ProjMatrixInv);

    _vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    _vector vRayDir = XMVectorSetW(vViewMousePos, 0.f);

    _matrix ViewMatrixInv = m_pGameInstance->Get_Transfrom_MatrixInverse(D3DTS::VIEW);

     XMStoreFloat3(&m_vRayPos[ENUM_TO_INT(RAY::WORLD)], XMVector3TransformCoord(vRayPos, ViewMatrixInv));
     XMStoreFloat3(&m_vRayDir[ENUM_TO_INT(RAY::WORLD)], XMVector3Normalize(XMVector3TransformNormal(vRayDir, ViewMatrixInv)));
}

void CPicking::Transform_ToLocalSpace(const FXMMATRIX pWorldMatrixInverse)
{
    XMStoreFloat3(&m_vRayPos[ENUM_TO_INT(RAY::LOCAL)], XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos[ENUM_TO_INT(RAY::WORLD)]), pWorldMatrixInverse));
    XMStoreFloat3(&m_vRayDir[ENUM_TO_INT(RAY::LOCAL)], XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir[ENUM_TO_INT(RAY::WORLD)]), pWorldMatrixInverse)));
}

_bool CPicking::Picking_InWorldSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut)
{
    _float fDist = {};

    _bool isCollision = TriangleTests::Intersects(
        XMLoadFloat3(&m_vRayPos[ENUM_TO_INT(RAY::WORLD)]), XMLoadFloat3(&m_vRayDir[ENUM_TO_INT(RAY::WORLD)]),
        vPointA, vPointB, vPointC, fDist);

    if (true == isCollision)
        XMStoreFloat3(pQut, XMLoadFloat3(&m_vRayPos[ENUM_TO_INT(RAY::WORLD)]) + XMLoadFloat3(&m_vRayDir[ENUM_TO_INT(RAY::WORLD)]) * fDist);

    return isCollision;
}

_bool CPicking::Picking_InLocalSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut)
{
    _float fDist = {};

    _bool isCollision = TriangleTests::Intersects(
        XMLoadFloat3(&m_vRayPos[ENUM_TO_INT(RAY::LOCAL)]), XMLoadFloat3(&m_vRayDir[ENUM_TO_INT(RAY::LOCAL)]),
        vPointA, vPointB, vPointC, fDist);

    if (true == isCollision)
        XMStoreFloat3(pQut, XMLoadFloat3(&m_vRayPos[ENUM_TO_INT(RAY::LOCAL)]) + XMLoadFloat3(&m_vRayDir[ENUM_TO_INT(RAY::LOCAL)]) * fDist);

    return isCollision;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
    CPicking* pInstance = new CPicking(pDevice, pContext);

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CPicking");
        return nullptr;
    }

    return pInstance;
}

void CPicking::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}
