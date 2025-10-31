#include "pch.h"
#include "Interaction.h"

#include "GameInstance.h"

CInteraction::CInteraction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CInteraction::CInteraction(const CInteraction& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CInteraction::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteraction::Initialize(void* pArg)
{
	UIOBJECT_DESC* pParent_Desc = static_cast<UIOBJECT_DESC*>(pArg);

	UIOBJECT_DESC Desc = {};

	Desc.fX = -30.f;
	Desc.fY = -2.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(_float3(0.4f, 0.4f, 0.4f));

	m_isVisible = false;

	return S_OK;
}

void CInteraction::Priority_Update(_float fTimeDelta)
{

}

void CInteraction::Update(_float fTimeDelta)
{
	

}

void CInteraction::Late_Update(_float fTimeDelta)
{
	if(m_isVisible)
	{
		m_pGameInstance->Add_FontDesc(TEXT("Bold_Font"), &m_FontDesc);
		m_pGameInstance->Add_RenderGroup(RENDER::WORLDUI, this);
	}
}

HRESULT CInteraction::Render()
{
	// UI
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CInteraction::Trigger_Interaction(_bool isActive, _uint iID, _fvector vPosition)
{
	if (true == isActive)
	{
		m_isVisible = true;
		_vector Position = XMVectorSet(vPosition.m128_f32[0] + 0.5f, vPosition.m128_f32[1] + 1.f, vPosition.m128_f32[2], 1.f);

		m_pTransformCom->Set_State(STATE::POSITION, Position);
		m_pTransformCom->BillBoard(XMLoadFloat4(m_pGameInstance->Get_Camera_Look()));

		Position = XMVector3TransformCoord(Position, XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW)));

		Position = XMVector3TransformCoord(Position, XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ)));

		m_FontDesc.vPositon.x = (_float)g_iWinSizeX * 0.5f + Position.m128_f32[0] * (_float)g_iWinSizeX * 0.5f - 60.f;
		m_FontDesc.vPositon.y = (_float)g_iWinSizeY * 0.5f - Position.m128_f32[1] * (_float)g_iWinSizeY * 0.5f - 10.f;

		if (0 == iID)
		{
			m_FontDesc.strWord = L"입장하기";
		}
		else
		{
			m_FontDesc.strWord = L"대화하기";
		}
	}
	else
	{
		m_isVisible = false;
	}
}

HRESULT CInteraction::Ready_Font()
{
	m_FontDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_FontDesc.strWord = L"대화";
	m_FontDesc.vPositon = _float2(590.f, 26.f);
	m_FontDesc.fScale = 0.31f;

	return S_OK;
}

HRESULT CInteraction::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Interaction"),
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

CInteraction* CInteraction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInteraction* pInstance = new CInteraction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CInteraction");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CInteraction::Clone(void* pArg)
{
	CGameObject* pInstance = new CInteraction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CInteraction");
		return nullptr;
	}
	return pInstance;
}

void CInteraction::Free()
{
	__super::Free();
}
