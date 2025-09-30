#include "pch.h"
#include "DialogueUI.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Npc.h"

CDialogueUI::CDialogueUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIPanel{ pDevice, pContext }
{
}

CDialogueUI::CDialogueUI(CDialogueUI& Prototype)
    :CUIPanel{ Prototype }
{
}

HRESULT CDialogueUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDialogueUI::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc = {};

    Desc.fX = 640.f;
    Desc.fY = 630.f;
    Desc.fZ = 0.1f;
    Desc.pParent_TransformCom = nullptr;
    Desc.fSizeX = 1281.f;
    Desc.fSizeY = 351.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Font()))
        return E_FAIL;

    if (FAILED(Ready_Scripts()))
        return E_FAIL;

    m_fX = 642.f;
    m_fY = 546.f;

    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 1.f, 1.f));
    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));

    _uint i = 1;
    D3D11_VIEWPORT Viewport = {};

    m_pContext->RSGetViewports(&i, &Viewport);
    m_fWinCX = Viewport.Width;
    m_fWinCY = Viewport.Height;

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

    m_iScriptIndex = 0;
    m_iTextureIndex = 0;

    return S_OK;
}

void CDialogueUI::Priority_Update(_float fTimeDelta)
{
}

void CDialogueUI::Update(_float fTimeDelta)
{
    if (m_pGameInstance->Get_KeyDown(DIK_G))
    {
        if (m_iScriptIndex < (_uint)m_Scripts.size() - 1)
            ++m_iScriptIndex;

        if (m_iScriptIndex == (_uint)m_Scripts.size() - 1)
            m_iTextureIndex = 1;
    }

    m_ScriptFontDesc.strWord = m_Scripts[m_iScriptIndex];
}

void CDialogueUI::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_FontDesc(TEXT("Nanum16_Font"), &m_NameFontDesc);
    m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_ScriptFontDesc);

    m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CDialogueUI::Render()
{
//#pragma region TEST_CODE
//    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 1.f, 1.f));
//    m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
//
//    ImGui::InputFloat("eX", &m_fX, 1.f, 10.f);
//    ImGui::InputFloat("eY", &m_fY, 1.f, 10.f);
//    ImGui::InputFloat("eSIZE X", &m_fSizeX, 1.f, 10.f);
//    ImGui::InputFloat("eSIZE Y", &m_fSizeY, 1.f, 10.f);
//
//    FONT_DESC* pFontDesc = &m_NameFontDesc;
//    ImGui::InputFloat("X", &pFontDesc->vPositon.x, 1.f, 10.f);
//    ImGui::InputFloat("Y", &pFontDesc->vPositon.y, 1.f, 10.f);
//    ImGui::InputFloat("Size", &pFontDesc->fScale, 0.01f, 0.1f);
//    ImGui::ColorEdit4(
//        "Color", (float*)&pFontDesc->vColor,
//        ImGuiColorEditFlags_AlphaBar
//        | ImGuiColorEditFlags_AlphaPreviewHalf
//        | ImGuiColorEditFlags_DisplayRGB
//        | ImGuiColorEditFlags_PickerHueWheel);
//
//    FONT_DESC* pFontDesc2 = &m_ScriptFontDesc;
//    ImGui::InputFloat("X2", &pFontDesc2->vPositon.x, 1.f, 10.f);
//    ImGui::InputFloat("Y2", &pFontDesc2->vPositon.y, 1.f, 10.f);
//    ImGui::InputFloat("Size2", &pFontDesc2->fScale, 0.01f, 0.1f);
//    ImGui::ColorEdit4(
//        "Color2", (float*)&pFontDesc2->vColor,
//        ImGuiColorEditFlags_AlphaBar
//        | ImGuiColorEditFlags_AlphaPreviewHalf
//        | ImGuiColorEditFlags_DisplayRGB
//        | ImGuiColorEditFlags_PickerHueWheel);
//
//#pragma endregion

    // UI
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    /* InCircle */
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(m_iTextureIndex))))
        return E_FAIL;

    if (FAILED(Draw_UI()))
        return E_FAIL;

    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDialogueUI::Ready_Components()
{
    /*Texture*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Texture_Dialogue"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /*VIBuffer_Rect*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /*Shader_VTXPosTex*/
    if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXPosTex"),
        TEXT("Com_Shader_VTXPosTex"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CDialogueUI::Ready_Font()
{
    m_NameFontDesc.vColor = _float4(0.46f, 1.f, 0.49f, 1.f);
    m_NameFontDesc.strWord = L"베아트리스";
    m_NameFontDesc.vPositon = _float2(601.f, 546.f);
    m_NameFontDesc.fScale = 0.53f;

    m_ScriptFontDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_ScriptFontDesc.strWord = L"빛의 길을 따라 저에게로 오세요";
    m_ScriptFontDesc.vPositon = _float2(547.f, 630.f);
    m_ScriptFontDesc.fScale = 0.4f;

    return S_OK;
}

HRESULT CDialogueUI::Ready_Scripts()
{
    m_Scripts.push_back(_wstring(L"구해주셔서 감사합니다."));
    m_Scripts.push_back(_wstring(L"카멘을 무찔러 주세요."));


    return S_OK;
}

HRESULT CDialogueUI::Draw_UI()
{
    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

CDialogueUI* CDialogueUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDialogueUI* pInstance = new CDialogueUI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CDialogueUI");
        return nullptr;
    }
    return pInstance;
}

CGameObject* CDialogueUI::Clone(void* pArg)
{
    CGameObject* pInstance = new CDialogueUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CDialogueUI");
        return nullptr;
    }
    return pInstance;
}

void CDialogueUI::Free()
{
    __super::Free();
 
}
