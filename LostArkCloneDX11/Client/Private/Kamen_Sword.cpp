#include "pch.h"
#include "Kamen_Sword.h"

#include "GameInstance.h"

#include "Kamen.h"
#include "Camera_Fix.h"

CKamen_Sword::CKamen_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEnemy{ pDevice, pContext }
{
}

CKamen_Sword::CKamen_Sword(const CKamen_Sword& Prototype)
	:CEnemy{ Prototype }, m_isAnimationFinished{ false }
{
}

HRESULT CKamen_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKamen_Sword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_EnemyInfo.fMaxHp = m_EnemyInfo.fHp = 100000.f;
	m_EnemyInfo.iMonsterID = 3;

#pragma region TEST_CODE
	m_EnemyInfo.fMaxHp = m_EnemyInfo.fHp = 10000.f;
#pragma endregion

	m_eCurState = m_ePreState = STATE::START;
	

	m_iNumMesh = m_pModelCom->Get_NumMeshes();

	m_pTransformCom->Set_State(Engine::STATE::POSITION, XMVectorSet(35.f, -0.2f, 60.f, 1.f));
	m_pTransformCom->Set_Scale(_float3(1.2f, 1.2f, 1.2f));
	m_pModelCom->Set_AnimationIndex(m_pTransformCom, 12, false);

	return S_OK;
}

void CKamen_Sword::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_Collider(TEXT("Monster"), m_pColliderCom);
}

void CKamen_Sword::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

	if (0.f >= m_EnemyInfo.fHp)
	{
		if (STATE::DEAD != m_eCurState)
			m_eCurState = STATE::DEAD;
		else
		{
			if (true == m_isAnimationFinished)
			{
				m_isDead = true;
				dynamic_cast<CKamen*>(m_pGameInstance->Get_LayerObjects(
					ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Kamen")).back())->Change_Phase(PHASE::PHASE1);

				m_pGameInstance->Find_Camera(TEXT("Camera_Fix"))->Set_LookDircetion(XMVectorSet(0.f, 7.f, -7.f, 0.f));
				m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_LookDircetion(XMVectorSet(0.f, 7.f, -7.f, 0.f));
				m_pGameInstance->Find_Camera(TEXT("Camera_ChargeSkill"))->Set_Fovy(60.f);

				m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"), false, 3.f);
			}
		}
		
	}

	if (STATE::START == m_eCurState && true == m_isAnimationFinished)
		m_eCurState = STATE::IDEL;

	m_isAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta);

	Change_State();
}

void CKamen_Sword::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CKamen_Sword::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transfrom_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &m_pGameInstance->Get_LightDesc(0).vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &m_pGameInstance->Get_LightDesc(0).vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &m_pGameInstance->Get_LightDesc(0).vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &m_pGameInstance->Get_LightDesc(0).vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPosition", m_pGameInstance->Get_Camera_Position(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_DiffuseTexture", m_pDiffuseTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Resource("g_EmissiveTexture", m_pEmissiveTextureCom->Get_SRV(0))))
		return E_FAIL;

	for (_uint i = 0; i < m_iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CKamen_Sword::Ready_Component()
{
	/* Prototype_Component_Texture_Sword_Kamen_Diffuse */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Sword_Kamen_Diffuse"),
		TEXT("Com_Texture_Diffuse"), reinterpret_cast<CComponent**>(&m_pDiffuseTextureCom))))
		return E_FAIL;

	/* Shader_VTXAnimTex */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Sword_Kamen_Emissive"),
		TEXT("Com_Texture_Emissive"), reinterpret_cast<CComponent**>(&m_pEmissiveTextureCom))))
		return E_FAIL;

	/* Shader_VTXAnimTex */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_Sword"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.vCenter = _float3(0.f, 0.5f, 0.f);
	Sphere_Desc.fRadius = 1.f;
	Sphere_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_HitBox_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Sphere_Desc)))
		return E_FAIL;

	return S_OK;
}

void CKamen_Sword::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CKamen_Sword::IDEL:
			m_pModelCom->Set_AnimationIndex(m_pTransformCom, 1, true);
			break;
		case Client::CKamen_Sword::HIT:
			m_pModelCom->Set_AnimationIndex(m_pTransformCom, 5, true);
			break;
		case Client::CKamen_Sword::DEAD:
			m_pModelCom->Set_AnimationIndex(m_pTransformCom, 14, false);
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CKamen_Sword* CKamen_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKamen_Sword* pInstance = new CKamen_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CKamen_Sword");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CKamen_Sword::Clone(void* pArg)
{
	CGameObject* pInstance = new CKamen_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CKamen_Sword");
		return nullptr;
	}

	return pInstance;
}

void CKamen_Sword::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pEmissiveTextureCom);
}
