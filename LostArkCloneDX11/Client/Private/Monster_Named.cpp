#include "pch.h"
#include "Monster_Named.h"

#include "GameInstance.h"

#include "Monster_Idle.h"
#include "Monster_Attack.h"
#include "Monster_Turn.h"
#include "Monster_Run.h"
#include "Monster_Dead.h"

CMonster_Named::CMonster_Named(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CMonster_Named::CMonster_Named(const CMonster_Named& Prototype)
	:CMonster{Prototype}
{
}

HRESULT CMonster_Named::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Named::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_iAnimIndex = 0;

	m_eType = MONSTER::NAMED;

	m_iNumMesh = m_pModelCom->Get_NumMeshes();
	Set_Animation(ANIMATIONSLOT::IDLE);

	m_pStateMachineCom->Start_State(m_States[IDLE]);

	return S_OK; 
}

void CMonster_Named::Priority_Update(_float fTimeDelta)
{
}

void CMonster_Named::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pStateMachineCom->Upadte(fTimeDelta);

	m_isAnimationFinish = m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster_Named::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CMonster_Named::Render()
{
#pragma region ANIMATION_TEST
	ImGui::InputInt("Animation", &m_iAnimIndex);
	_int iIndex = {};
	for (auto pName : m_pModelCom->Get_AnimationNames())
	{
		if (ImGui::Button(to_string(iIndex).c_str()))
		{
			m_iAnimIndex = iIndex;
			m_pModelCom->Set_AnimationIndex(m_pTransformCom, m_iAnimIndex, true);
		}
		++iIndex;
		ImGui::SameLine();
		ImGui::Text(pName);
	}
#pragma endregion

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 0; i < m_iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(i, m_pShaderCom, "g_DiffuseTexture", TEXTURE::DIFFUSE, 0, "g_DiffuseColor")))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}
HRESULT CMonster_Named::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* StateMachine */
	if (FAILED(__super::Add_Component(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Named::Ready_States()
{
	m_States[IDLE] = CMonster_Idle::Create(m_pStateMachineCom, &m_eType, this);
	m_States[ATTACK] = CMonster_Attack::Create(m_pStateMachineCom, &m_eType, this, m_iNumAttack);
	m_States[TURN] = CMonster_Turn::Create(m_pStateMachineCom, &m_eType, this);
	m_States[RUN] = CMonster_Run::Create(m_pStateMachineCom, &m_eType, this);
	m_States[DEAD] = CMonster_Dead::Create(m_pStateMachineCom, &m_eType, this);

	return S_OK;
}

CMonster_Named* CMonster_Named::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Named* pInstance = new CMonster_Named(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CMonster_Stancd ");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster_Named::Clone(void* pArg)
{
	CGameObject* pInstance = new CMonster_Named(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CMonster_Stancd ");
		return nullptr;
	}

	return pInstance;
}

void CMonster_Named::Free()
{
	__super::Free();
}