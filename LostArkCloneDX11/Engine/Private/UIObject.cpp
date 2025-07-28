#include "UIObject.h"

#include "GameInstance.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice, pContext}
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
    : CGameObject{Prototype}
{
}

HRESULT CUIObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fX = pDesc->fX;
    m_fY = pDesc->fY;
    m_fZ = pDesc->fZ;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;

    if (nullptr == pDesc->pParent_TransformCom)
        m_pParent_TransformCom = m_pTransformCom;
    else
        m_pParent_TransformCom = pDesc->pParent_TransformCom;

    Safe_AddRef(m_pParent_TransformCom);

    Update_Position();

    return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
    return S_OK;
}

void CUIObject::Update_Position()
{
    _uint i = 1;
    D3D11_VIEWPORT Viewport = {};
    
    m_pContext->RSGetViewports(&i, &Viewport);
    _float fWinCX = Viewport.Width;
    _float fWinCY = Viewport.Height;

    _float3 vParentPosition = {};
    XMStoreFloat3(&vParentPosition, m_pParent_TransformCom->Get_Position());

    _float3 vPosition = { vParentPosition.x + m_fX, vParentPosition.y - m_fY , 0.f };

    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat3(&vPosition));
}

void CUIObject::Free()
{
    __super::Free();

    Safe_Release(m_pParent_TransformCom);
}
