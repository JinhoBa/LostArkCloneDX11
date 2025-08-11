#include "pch.h"
#include "Background_Logo.h"

#include "GameInstance.h"

CBackground_Logo::CBackground_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel{pDevice, pContext}
{
}

CBackground_Logo::CBackground_Logo(const CBackground_Logo& Prototype)
	: CUIPanel{Prototype}
{
}

HRESULT CBackground_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackground_Logo::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc = {};

	Desc.fX = 0.f;
	Desc.fY = 0.f;
	Desc.fZ = 0.9f;
	Desc.fSizeX = (_float)g_iWinSizeX;
	Desc.fSizeY = (_float)g_iWinSizeY;
	Desc.pParent_TransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		ENUM_TO_INT(LEVEL::STATIC),TEXT("Layer_Canvars"), TEXT("Com_Transform")
	));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackground_Logo::Priority_Update(_float fTimeDelta)
{
}

void CBackground_Logo::Update(_float fTimeDelta)
{
	m_pAnimCom->Update(fTimeDelta);
}

void CBackground_Logo::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CBackground_Logo::Render()
{
	if (FAILED(__super::Bind_ShaderResource(m_pAnimCom->Get_Frame())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
	    return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
	    return E_FAIL;

	return S_OK;
}

HRESULT CBackground_Logo::Add_Components()
{
	/*Texture*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LogoBackGround"),
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

	CUIAnimation::UIANIM_DESC Anim_Desc = {};
	Anim_Desc.bLoop = true;
	Anim_Desc.fAnimTime = 5.f;
	Anim_Desc.iStartFrame = 0;
	Anim_Desc.iEndFrame = 149;
	Anim_Desc.pShaderCom = m_pShaderCom;
	Anim_Desc.pTextureCom = m_pTextureCom;

	/*Animation*/
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::STATIC), TEXT("Prototype_Component_UIAnimation"),
		TEXT("Com_UIAnimation"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Anim_Desc)))
		return E_FAIL;


	return S_OK;
}

CBackground_Logo* CBackground_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackground_Logo* pInstance = new CBackground_Logo(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Create : CBackground_Logo");
        return nullptr;
    }
    return pInstance;
}

CGameObject* CBackground_Logo::Clone(void* pArg)
{
	CGameObject* pInstance = new CBackground_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CBackground_Logo");
		return nullptr;
	}
	return pInstance;
}

void CBackground_Logo::Free()
{
	__super::Free();

	Safe_Release(m_pAnimCom);
}
