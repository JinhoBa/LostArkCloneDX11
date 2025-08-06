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
 
    _uint i = 1;
    D3D11_VIEWPORT Viewport = {};

    m_pContext->RSGetViewports(&i, &Viewport);
    m_fWinCX = Viewport.Width;
    m_fWinCY = Viewport.Height;

    CTransform::Transform_Desc Transform_Desc = {};
    Transform_Desc.fRotatePersec = 1.f;
    Transform_Desc.fSpeedPersec = 1.f;

    if (FAILED(__super::Initialize(&Transform_Desc)))
        return E_FAIL;

    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fRX = pDesc->fX;
    m_fRY = pDesc->fY;
    m_fZ = pDesc->fZ;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;

    if (nullptr == pDesc->pParent_TransformCom)
        m_pParent_TransformCom = m_pTransformCom;
    else
        m_pParent_TransformCom = pDesc->pParent_TransformCom;

    Safe_AddRef(m_pParent_TransformCom);

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

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
    m_ChildObjects.sort(
        [](CUIObject* pSrc, CUIObject* pDst)->_bool {return pSrc->Get_ZValue() < pDst->Get_ZValue();}
    );
}

HRESULT CUIObject::Render()
{
    for (auto pObject : m_ChildObjects)
    {
        pObject->Render();
    }

    return S_OK;
}

void CUIObject::Update_Position()
{
    _float3 vParentPosition = {};
    XMStoreFloat3(&vParentPosition, m_pParent_TransformCom->Get_Position());

    m_fX = vParentPosition.x + (m_fWinCX * 0.5f) + m_fRX;
    m_fY = -vParentPosition.y + (m_fWinCY * 0.5f) + m_fRY;

    _float4 vPosition = { m_fX - (m_fWinCX * 0.5f) , -m_fY + (m_fWinCY * 0.5f), m_fZ - 0.01f, 1.f };

    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&vPosition));
}

HRESULT CUIObject::Add_ChildObjects(_uint iLevelID, const _wstring& strLayerTag)
{
    list<CGameObject*>* ChildList = m_pGameInstance->Get_LayerObjects(iLevelID, strLayerTag);

    if (nullptr == ChildList)
        return S_OK;

    for (auto pGameObject : *ChildList)
    {
        m_ChildObjects.push_back(dynamic_cast<CUIObject*>(pGameObject));
        Safe_AddRef(pGameObject);
    }

    return S_OK;
}

void CUIObject::Free()
{
    __super::Free();

    Safe_Release(m_pParent_TransformCom);

    for (auto pObject : m_ChildObjects)
    {
        Safe_Release(pObject);
    }
    m_ChildObjects.clear();
}
