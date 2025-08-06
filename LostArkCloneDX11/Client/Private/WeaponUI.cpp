#include "pch.h"
#include "WeaponUI.h"

#include "GameInstance.h"

CWeaponUI::CWeaponUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CWeaponUI::CWeaponUI(const CWeaponUI& Prototype)
	: CUIPanel{ Prototype },
	m_iNumShake{0},
	m_fTimeAcc{0.f}
{
}

HRESULT CWeaponUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeaponUI::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	UIOBJECT_DESC Desc = {};

	Desc.fX = 4.f;
	Desc.fY = 51.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 100.f;
	Desc.fSizeY = 210.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_ePreState = STATE::TO_FLURRY;
	m_eCurState = STATE::TO_FLURRY;

	m_iTextureIndex = 0;
	m_fShakeSpeed = 15.f;
	

	return S_OK;
}

void CWeaponUI::Priority_Update(_float fTimeDelta)
{
}

void CWeaponUI::Update(_float fTimeDelta)
{
#pragma region MyRegion
	if (m_pGameInstance->KeyDown('Z'))
	{
		if (0 == m_iTextureIndex)
		{
			m_eCurState = STATE::TO_FOCUS;
			m_iTextureIndex = 1;
		}
		else
		{
			m_eCurState = STATE::TO_FLURRY;
			m_iTextureIndex = 0;
		}
	}
	
	m_fRY += fTimeDelta;

#pragma endregion


	switch (m_eCurState)
	{
	case Client::CWeaponUI::STATE::FLURRY:
		break;

	case Client::CWeaponUI::STATE::FOCUS:
		break;

	case Client::CWeaponUI::STATE::TO_FLURRY:
		Shake(fTimeDelta);
		break;

	case Client::CWeaponUI::STATE::TO_FOCUS:
		break;

	default:
		break;
	}

	Change_State();

	Update_Position();
}

void CWeaponUI::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeaponUI::Render()
{
	if (FAILED(__super::Bind_ShaderResource(m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponUI::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Weapon"),
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

void CWeaponUI::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CWeaponUI::STATE::FLURRY:
			m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(0.f));
			m_iNumShake = 0;
			break;

		case Client::CWeaponUI::STATE::FOCUS:
			break;

		case Client::CWeaponUI::STATE::TO_FLURRY:
			m_fTurnTime = 0.07f;
			m_fShakeSpeed = 15.f;
			break;

		case Client::CWeaponUI::STATE::TO_FOCUS:
			break;

		default:
			break;
		}
		m_ePreState = m_eCurState;
		m_fTimeAcc = 0.f;
	}

}

void CWeaponUI::Shake(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= m_fTurnTime)
	{
		m_fShakeSpeed *= -0.9f;
		++m_iNumShake;
		m_fTimeAcc = 0.f;
	}
	m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta * m_fShakeSpeed);

	if (m_iNumShake > 6)
		m_eCurState = STATE::FLURRY;
}

void CWeaponUI::UpAndDown(_float fTimeDelta)
{

}

CWeaponUI* CWeaponUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeaponUI* pInstance = new CWeaponUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CWeaponUI");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWeaponUI::Clone(void* pArg)
{
	CGameObject* pInstance = new CWeaponUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CWeaponUI");
		return nullptr;
	}
	return pInstance;
}

void CWeaponUI::Free()
{
	__super::Free();
}
