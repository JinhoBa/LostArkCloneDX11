#include "pch.h"
#include "Mouse.h"

#include "GameInstance.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CMouse::CMouse(const CMouse& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CMouse::Initialize_Prototype()
{
	//ShowCursor(FALSE);

	return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = -220.f;
	Desc.fZ = 0.5f;
	Desc.fSizeX = 40.f;
	Desc.fSizeY = 40.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
}

void CMouse::Update(_float fTimeDelta)
{
	POINT pt = m_pGameInstance->Get_MousePoint();

	m_fX = (_float)pt.x + 15.f;
	m_fY = (_float)pt.y + 20.f;


	_float4 vPosition = { m_fX - (m_fWinCX * 0.5f) , -m_fY + (m_fWinCY * 0.5f), 1.f, 1.f };

	m_pTransformCom->Set_Scale(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&vPosition));
}

void CMouse::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(RENDER::CURSOR, this);
}

HRESULT CMouse::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_Texture2D", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Cursor"),
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

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMouse* pInstance = new CMouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMouse");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
	CGameObject* pInstance = new CMouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CMouse");
		return nullptr;
	}
	return pInstance;
}

void CMouse::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
