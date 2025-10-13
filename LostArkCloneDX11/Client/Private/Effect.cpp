#include "pch.h"
#include "Effect.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Skill.h"
#include "Camera_Fix.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CEffect::CEffect(const CEffect& Prototype)
    :CPartObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
    m_fLifeTime = 0.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{
}

void CEffect::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffect::Render()
{
    return S_OK;
}

HRESULT CEffect::Start(void* pArg)
{
    return S_OK;
}

HRESULT CEffect::Reset()
{
    return S_OK;
}


void CEffect::Free()
{
    __super::Free();
}
