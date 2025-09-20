#include "pch.h"
#include "Enemy.h"

CEnemy::CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCharacter{pDevice, pContext}
{
}

CEnemy::CEnemy(const CEnemy& Prototype)
    :CCharacter{Prototype}
{
}

HRESULT CEnemy::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEnemy::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CEnemy::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CEnemy::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CEnemy::Render()
{
    return S_OK;
}

void CEnemy::OnHit(const ATTACK_DESC& Attack_Desc)
{
    m_EnemyInfo.fHp -= Attack_Desc.fDamage;

    m_HitTypes.push_back(Attack_Desc.eHitType);
}

void CEnemy::Free()
{
    __super::Free();
}
