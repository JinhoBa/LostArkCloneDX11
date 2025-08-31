#include "pch.h"
#include "HUD.h"

#include "GameInstance.h"

#include "Player.h"

CHUD::CHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIPanel{ pDevice , pContext }
{
}

CHUD::CHUD(const CHUD& Prototype)
    :CUIPanel{Prototype}
{
}

HRESULT CHUD::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHUD::Initialize(void* pArg)
{
    m_pPlayerInfo = dynamic_cast<CPlayer*>(
        m_pGameInstance->Get_LayerObjects(
            ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player"))->back())->Get_Info();

    if (nullptr == m_pPlayerInfo)
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CHUD::Priority_Update(_float fTimeDelta)
{
}

void CHUD::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CHUD::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CHUD::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

void CHUD::Free()
{
    __super::Free();
}
