#include "pch.h"
#include "Iden_Gauge.h"

#include "GameInstance.h"

CIden_Gauge::CIden_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CIden_Gauge::CIden_Gauge(const CIden_Gauge& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CIden_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIden_Gauge::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	UIOBJECT_DESC Desc = {};

	Desc.fX = 3.f;
	Desc.fY = 14.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 115.f;
	Desc.fSizeY = 115.f;
	Desc.pParent_TransformCom = static_cast<UIOBJECT_DESC*>(pArg)->pParent_TransformCom;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;



	return S_OK;
}

void CIden_Gauge::Priority_Update(_float fTimeDelta)
{
}

void CIden_Gauge::Update(_float fTimeDelta)
{
	m_pAnimationCom->Update(fTimeDelta);
}

void CIden_Gauge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CIden_Gauge::Render()
{
	// Fire
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pFireTextureCom->Get_SRV(m_pAnimationCom->Get_Frame()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	// Gauge
	if (FAILED(__super::Bind_ShaderResource(1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIden_Gauge::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Gauge"),
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

	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Gauge_Fire"),
		TEXT("Com_FireTexture"), reinterpret_cast<CComponent**>(&m_pFireTextureCom))))
		return E_FAIL;

	CUIAnimation::UIANIM_DESC Anim_Desc = {};
	Anim_Desc.bLoop = true;
	Anim_Desc.fAnimTime = 0.8f;
	Anim_Desc.iStartFrame = 0;
	Anim_Desc.iEndFrame = 11;
	Anim_Desc.pShaderCom = m_pShaderCom;
	Anim_Desc.pTextureCom = m_pFireTextureCom;

	/*Animation*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_UIAnimation"),
		TEXT("Com_UIAnimation"), reinterpret_cast<CComponent**>(&m_pAnimationCom), &Anim_Desc)))
		return E_FAIL;


	return S_OK;
}

CIden_Gauge* CIden_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIden_Gauge* pInstance = new CIden_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CIden_Gauge");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CIden_Gauge::Clone(void* pArg)
{
	CGameObject* pInstance = new CIden_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CIden_Gauge");
		return nullptr;
	}
	return pInstance;
}

void CIden_Gauge::Free()
{
	__super::Free();

	Safe_Release(m_pFireTextureCom);
	Safe_Release(m_pAnimationCom);
}
