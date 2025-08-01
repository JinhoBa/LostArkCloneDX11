#include "UIAnimation.h"

#include "GameInstance.h"

CUIAnimation::CUIAnimation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{pDevice, pContext}
{
}

CUIAnimation::CUIAnimation(CUIAnimation& Prototype)
    : CComponent{Prototype}, m_bLoop{false}
{
}

HRESULT CUIAnimation::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIAnimation::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    UIANIM_DESC* pDesc = static_cast<UIANIM_DESC*>(pArg);

    m_bLoop = pDesc->bLoop;
    m_iCurFrame = m_iStartFrame = pDesc->iStartFrame;
    m_iEndFrame = pDesc->iEndFrame;
    m_fAnimTime = pDesc->fAnimTime;
    m_pTextureCom = pDesc->pTextureCom;
    
    Safe_AddRef(m_pTextureCom);

    m_fTimeAcc = 0.f;
    m_fFrameTime = m_fAnimTime / (m_iEndFrame - m_iStartFrame + 1);

    return S_OK;
}

void CUIAnimation::Update(_float fTimeDelta)
{
    if (m_iCurFrame < m_iEndFrame)
    {
        m_fTimeAcc += fTimeDelta;
        if (m_fFrameTime <= m_fTimeAcc)
        {
            ++m_iCurFrame;
            m_fTimeAcc = 0.f;
        }
    }
    else if (true == m_bLoop)
    {
        m_iCurFrame = m_iStartFrame;
    }

}

void CUIAnimation::Draw()
{
   
}

CUIAnimation* CUIAnimation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CComponent* CUIAnimation::Clone(void* pArg)
{
    return nullptr;
}

void CUIAnimation::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
}
