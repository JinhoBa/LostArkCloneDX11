#include "pch.h"
#include "Identity.h"

#include "GameInstance.h"

CIdentity::CIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CIdentity::CIdentity(const CIdentity& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CIdentity::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIdentity::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 25.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 200.f;
	Desc.fSizeY = 200.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;




	return S_OK;
}

void CIdentity::Priority_Update(_float fTimeDelta)
{
}

void CIdentity::Update(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case Client::CIdentity::STATE::FLURRY:
		break;

	case Client::CIdentity::STATE::FOCUS:
		break;

	case Client::CIdentity::STATE::TO_FLURRY:
		break;

	case Client::CIdentity::STATE::TO_FOCUS:
		break;

	default:
		break;
	}

	Change_State();

	Update_Position();
}

void CIdentity::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CIdentity::Render()
{
	if (FAILED(__super::Bind_ShaderResource(0)))
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

HRESULT CIdentity::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Frame"),
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

void CIdentity::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CIdentity::STATE::FLURRY:
			break;
		case Client::CIdentity::STATE::FOCUS:
			break;
		case Client::CIdentity::STATE::TO_FLURRY:
			break;
		case Client::CIdentity::STATE::TO_FOCUS:
			break;
	
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}

}

CIdentity* CIdentity::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIdentity* pInstance = new CIdentity(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIdentity");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CIdentity::Clone(void* pArg)
{
	CGameObject* pInstance = new CIdentity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CIdentity");
		return nullptr;
	}
	return pInstance;
}

void CIdentity::Free()
{
	__super::Free();
}
