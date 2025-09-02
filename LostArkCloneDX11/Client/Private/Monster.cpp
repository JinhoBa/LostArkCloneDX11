#include "pch.h"
#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{
}

CMonster::CMonster(const CMonster& Prototype) 
	: CGameObject { Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{		
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CMonster::Update(_float fTimeDelta)
{
	/*m_pModelCom->Play_Animation(fTimeDelta);*/
}

void CMonster::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CMonster::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;




	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

	/*	if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, 0)))
			return E_FAIL;*/
		/*if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, 0)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
		

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
		return E_FAIL;




	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
