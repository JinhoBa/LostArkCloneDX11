#include "pch.h"
#include "Background_Loading.h"

#include "GameInstance.h"

#include "Font.h"

CBackground_Loading::CBackground_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBackground_Loading::CBackground_Loading(const CBackground_Loading& Prototype)
	: CUIPanel{ Prototype }
{
}

HRESULT CBackground_Loading::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackground_Loading::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.fZ = 0.7f;
	Desc.fSizeX = (_float)g_iWinSizeX;
	Desc.fSizeY = (_float)g_iWinSizeY;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC), TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	Reay_FontDesc();

	return S_OK;
}

void CBackground_Loading::Priority_Update(_float fTimeDelta)
{
}

void CBackground_Loading::Update(_float fTimeDelta)
{
}

void CBackground_Loading::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &m_Font_Tip);
	m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &m_Font_TipDetail);
	m_pGameInstance->Add_FontDesc(TEXT("Defualt_Font"), &m_Font_Mococo);
}

HRESULT CBackground_Loading::Render()
{
	if (FAILED(__super::Bind_ShaderResource(0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBackground_Loading::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBackGround"),
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

void CBackground_Loading::Reay_FontDesc()
{
	m_Font_Tip.strWord = wstring(L"팁");
	m_Font_Tip.vPositon = _float4(966.f, 815.f, 1.f, 1.f);
	m_Font_Tip.vColor = _float4(0.9f, 0.8f, 0.54f, 1.f);
	m_Font_Tip.fScale = 0.57f;

	m_Font_TipDetail.strWord = wstring(L"부위 파괴 스킬로 적을 약화 시킬 수 있습니다.");
	m_Font_TipDetail.vPositon = _float4(765.f, 875.f, 1.f, 1.f);
	m_Font_TipDetail.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
	m_Font_TipDetail.fScale = 0.59f;

	m_Font_Mococo.strWord = wstring(L"154               99");
	m_Font_Mococo.vPositon = _float4(888.f, 109.f, 1.f, 1.f);
	m_Font_Mococo.vColor = _float4(0.7f, 0.7f, 0.7f, 1.f);
	m_Font_Mococo.fScale = 0.47f;
}

CBackground_Loading* CBackground_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackground_Loading* pInstance = new CBackground_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBackground_Loading");
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBackground_Loading::Clone(void* pArg)
{
	CGameObject* pInstance = new CBackground_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBackground_Loading");
		return nullptr;
	}
	return pInstance;
}

void CBackground_Loading::Free()
{
	__super::Free();

}
