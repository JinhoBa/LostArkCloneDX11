#include "pch.h"
#include "BossEnterUI.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "Kamen.h"
#include "Player.h"
#include "Effect_BossEnter.h"

CBossEnterUI::CBossEnterUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBossEnterUI::CBossEnterUI(const CBossEnterUI& Prototype)
	: CUIPanel{ Prototype },
	m_isVisible{ false }
{
}

HRESULT CBossEnterUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossEnterUI::Initialize(void* pArg)
{
	UIOBJECT_DESC* pParent_Desc = static_cast<UIOBJECT_DESC*>(pArg);

	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = -180.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 820.f;
	Desc.fSizeY = 90.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	m_pBossEnterEffect = dynamic_cast<CEffect_BossEnter*>(
		m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_BossEnter")).back());

	if (nullptr == m_pBossEnterEffect)
		return E_FAIL;

	return S_OK;
}

void CBossEnterUI::Priority_Update(_float fTimeDelta)
{
}

void CBossEnterUI::Update(_float fTimeDelta)
{
	if (m_pBossEnterEffect->isEnter())
	{
		m_fTimeAcc += fTimeDelta;
		_uint iCount = 5 - (_uint)m_fTimeAcc;
		m_CountFontDesc.strWord = to_wstring(iCount) + L"초";
		m_isVisible = true;

		if (0 == iCount)
		{
			m_pBossEnterEffect->Set_Dead();
			Enter_Boss();
			m_isDead = true;
		}
		else if (m_iPreCount != iCount)
		{
			m_pGameInstance->Play_Sound(L"ClockSingle.wav", CHANNELID::SYSTEM, 1.f);
			m_iPreCount = iCount;
		}

	}
	else
	{
		m_isVisible = false;
		m_fTimeAcc = 0.f;
	}

}

void CBossEnterUI::Late_Update(_float fTimeDelta)
{
	if (m_isVisible)
	{
		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_MessageFontDesc);
		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_CountFontDesc);
	}

	__super::Late_Update(fTimeDelta);

	if(!m_isDead)
		m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CBossEnterUI::Render()
{
	if(m_isVisible)
	{
		// UI
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CBossEnterUI::Ready_Font()
{
	m_MessageFontDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_MessageFontDesc.strWord = L"잠시 후 다음 지점으로 이동됩니다. ";
	m_MessageFontDesc.vPositon = _float2(500.f, 145.f);
	m_MessageFontDesc.fScale = 0.48f;

	m_CountFontDesc.vColor = _float4(0.95f, 0.86f, 0.f, 1.f);
	m_CountFontDesc.strWord = L"8초";
	m_CountFontDesc.vPositon = _float2(598.f, 184.f);
	m_CountFontDesc.fScale = 0.55f;

	return S_OK;
}

HRESULT CBossEnterUI::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Message_Background"),
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

HRESULT CBossEnterUI::Enter_Boss()
{
	CGameManager::GetInstance()->FadeOut(2.f);

	m_pGameInstance->Bind_Camera(TEXT("Camera_Enter"));

	dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->EnterBoss();
	dynamic_cast<CKamen*>(m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Kamen")).back())->Set_Animation(182, false);

	return S_OK;
}

CBossEnterUI* CBossEnterUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossEnterUI* pInstance = new CBossEnterUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBossEnterUI");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBossEnterUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CBossEnterUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBossEnterUI");
		return nullptr;
	}
	return pInstance;
}

void CBossEnterUI::Free()
{
	__super::Free();

	Safe_Release(m_pBossEnterEffect);
}
