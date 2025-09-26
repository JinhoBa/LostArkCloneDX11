#include "pch.h"
#include "DamageFont_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"
#include "Monster.h"
#include "Kamen.h"
#include "Player.h"

CDamageFont_Manager::CDamageFont_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() },
    m_pGameManager{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGameManager);
}

HRESULT CDamageFont_Manager::Initilize()
{
 
    return S_OK;
}

void CDamageFont_Manager::Add_DamageFont(DAMAGEFONT eType, _float fDamage, _float3& vPostion)
{
    FONT_DESC DamageFont = {};
    
    _int iDamage = (_int)(fDamage * m_pGameInstance->Random(0.8f, 1.2f));
    DamageFont.strWord = to_wstring(iDamage);
    DamageFont.fScale = 1.5f;

    switch (eType)
    {
    case Client::DAMAGEFONT::NORMAL:
        DamageFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
        break;

    case Client::DAMAGEFONT::CRITICAL:
        DamageFont.vColor = _float4(0.9f,0.8f, 0.11f, 1.f);
        break;

    case Client::DAMAGEFONT::PLAYER_HURT:
        DamageFont.vColor = _float4(0.8f, 0.2f, 0.2f, 1.f);
        DamageFont.fScale = 1.0f;
        break;
    }
    DamageFont.vPositon = _float2(vPostion.x, vPostion.y);
    _float OffSetX = m_pGameInstance->Random(-0.5f, 0.5f);
    _float OffSetXZ = m_pGameInstance->Random(-0.5f, 0.5f);
    _float4 vPosition = _float4(vPostion.x + OffSetX, vPostion.y + 2.f, vPostion.z + OffSetXZ, 0.f);

    m_DamageFonts.emplace_back(DamageFont, vPosition);
}

void CDamageFont_Manager::Update_DamageFont(_float fTimeDelta)
{
    if (m_DamageFonts.empty())
        return;

    auto iter = m_DamageFonts.begin();

    for (; iter != m_DamageFonts.end();)
    {
        (*iter).second.w += fTimeDelta;

        if (0.5f < (*iter).first.fScale)
            (*iter).first.fScale -= 0.05f;

        (*iter).first.vColor.w = 1.f - max(0.f ,(*iter).second.w - 0.8f) * 2.f;
        (*iter).first.vColor.x *= (*iter).first.vColor.w;
        (*iter).first.vColor.y *= (*iter).first.vColor.w;
        (*iter).first.vColor.z *= (*iter).first.vColor.w;
        (*iter).first.vColor.w *= (*iter).first.vColor.w;

        _vector ViewPortPosition = XMVector3TransformCoord(XMVectorSet((*iter).second.x, (*iter).second.y, (*iter).second.z, 0.f), m_pGameInstance->Get_Transfrom_Matrix(D3DTS::VIEW));
        ViewPortPosition = XMVector3TransformCoord(ViewPortPosition, m_pGameInstance->Get_Transfrom_Matrix(D3DTS::PROJ));

        (*iter).first.vPositon = _float2(
            (_float)g_iWinSizeX * 0.5f + ViewPortPosition.m128_f32[0] * (_float)g_iWinSizeX * 0.5f- 50.f,
            (_float)g_iWinSizeY * 0.5f + ViewPortPosition.m128_f32[1] * (_float)g_iWinSizeY * -0.5f);

        m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &(*iter).first);

        if ((*iter).second.w > 1.f)
        {
            iter = m_DamageFonts.erase(iter);
        }
        else
            iter++;
    }
}

CDamageFont_Manager* CDamageFont_Manager::Create()
{
    CDamageFont_Manager* pInstance = new CDamageFont_Manager();

    if(FAILED(pInstance->Initilize()))
        return nullptr;

    return pInstance;
}

void CDamageFont_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGameManager);
}
