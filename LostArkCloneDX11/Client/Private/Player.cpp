#include "pch.h"
#include "Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{
}

void CPlayer::Late_Update(_float fTimeDelta)
{

}

HRESULT CPlayer::Render()
{


    return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CPlayer");
        return nullptr;
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CGameObject* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Clone : CPlayer");
        return nullptr;
    }

    return pInstance;
}

void CPlayer::Free()
{
    __super::Free();
}
