#include "pch.h"
#include "GameManager.h"

#include "GameInstance.h"

#include "Data_Manager.h"
#include "Skill_Manager.h"
#include "Buff_Manager.h"
#include "DamageFont_Manager.h"
#include "Clash_Manager.h"
#include "Dialogue_Controller.h"

#include "UIObject.h"
#include "Camera_Fix.h"
#include "Terrain.h"
#include "Buff.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
}

HRESULT CGameManager::Initialize_Manager()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	m_pData_Manager = CData_Manager::Create();
	if (nullptr == m_pData_Manager)
		return E_FAIL;

	if (FAILED(m_pData_Manager->Load_SkillData("../Bin/Resources/Data/Skill_Data_V2.xml")))
		return E_FAIL;

	m_pSkill_Manager = CSkill_Manager::Create();
	if (nullptr == m_pSkill_Manager)
		return E_FAIL;

	m_pBuff_Manager = CBuff_Manager::Create();
	if (nullptr == m_pBuff_Manager)
		return E_FAIL;

	m_pDamageFont_Manager = CDamageFont_Manager::Create();
	if (nullptr == m_pDamageFont_Manager)
		return E_FAIL;

	m_pClash_Manager = CClash_Manager::Create();
	if (nullptr == m_pClash_Manager)
		return E_FAIL;

	m_pDialogue_Controller = CDialogue_Controller::Create();
	if (nullptr == m_pDialogue_Controller)
		return E_FAIL;

	return S_OK;
}

void CGameManager::Destory_GameManager()
{
	DestroyInstance();

	Safe_Release(m_pDialogue_Controller);
	Safe_Release(m_pClash_Manager);
	Safe_Release(m_pDamageFont_Manager);
	Safe_Release(m_pBuff_Manager);
	Safe_Release(m_pSkill_Manager);
	Safe_Release(m_pData_Manager);

	Safe_Release(m_pGameInstance);
}


#pragma region DATA_MANAGER

const vector<MAP_DATA>& CGameManager::Get_MapData()
{
	return m_pData_Manager->Get_MapData();
}

const vector<TERRAIN_DATA>& CGameManager::Get_TerrainData()
{
	return m_pData_Manager->Get_TerrainData();
}

HRESULT CGameManager::Load_MapData(const _char* pMapDataFilePaht)
{
	m_pData_Manager->Load_MapData(pMapDataFilePaht);

	return S_OK;
}

HRESULT CGameManager::Save_MapData(const _char* pFileName)
{
	m_pData_Manager->Save_MapData(pFileName);

	return S_OK;
}

HRESULT	 CGameManager::Load_SkillData(const _char* pFilePath)
{
	return m_pData_Manager->Load_SkillData(pFilePath);
}

SKILL_INFO* CGameManager::Get_SkillInfo_Prt(_uint iSkillID)
{
	return m_pData_Manager->Get_SkillInfo_Prt(iSkillID);
}

HRESULT	CGameManager::Load_Monster_SkillData(const _char* pFilePath)
{
	return m_pData_Manager->Load_Monster_SkillData(pFilePath);
}
MONSTER_SKILL_INFO* CGameManager::Get_Monster_SkillInfo_Prt(_uint iMonsterID, _uint iSkillID)
{
	return m_pData_Manager->Get_Monster_SkillInfo_Prt(iMonsterID, iSkillID);
}

#pragma endregion

#pragma region SKILL_MANAGER
void CGameManager::Set_PlayerInfoPrt(PLAYER_INFO* pPlayerInfo)
{
	m_pSkill_Manager->Set_PlayerInfoPrt(pPlayerInfo);
}

void CGameManager::Update_Skills(_float fTimeDelta)
{
	m_pSkill_Manager->Update(fTimeDelta);
}

const _float CGameManager::Check_CoolTime(_uint iSkillID) const
{
	if (99 == iSkillID)
		return 0.f;
	return m_pSkill_Manager->Check_CoolTime(iSkillID);
}

const _bool	CGameManager::Use_Skill(_uint iSkillID) const
{
	return m_pSkill_Manager->Use_Skill(iSkillID);
}

#pragma endregion

#pragma region ANIMATION_DATA
ANIMATION_DESC& CGameManager::Get_AnimationIndex(_uint iMonsterID, ANIMATIONSLOT eSlot)
{
	return m_pData_Manager->Get_AnimationIndex(iMonsterID, eSlot);
}
HRESULT	CGameManager::Load_AnimationData(const _char* pFilePath)
{
	return m_pData_Manager->Load_AnimationData(pFilePath);
}
#pragma endregion

#pragma region KAMEN_DATA
MONSTER_SKILL_INFO* CGameManager::Get_KamenData(_uint iPhase, _uint iSkillID)
{
	return m_pData_Manager->Get_KamenData(iPhase, iSkillID);
}

HRESULT	CGameManager::Load_KamenData(const _char* pFilePath)
{
	return m_pData_Manager->Load_KamenData(pFilePath);
}
#pragma endregion

#pragma region CAMERA_ANIMATION
const CAMERA_ANIMATION_DESC* CGameManager::Get_CameraAnimation(_uint iID)
{
	return m_pData_Manager->Get_CameraAnimation(iID);
}

HRESULT CGameManager::Load_CameraAnimation(const _char* pFilePath)
{
	return m_pData_Manager->Load_CameraAnimation(pFilePath);
}
#pragma endregion

#pragma region BUFF_MANAGER

CBuff* CGameManager::Add_Buff(_uint iBuffID)
{
	return m_pBuff_Manager->Add_Buff(iBuffID);
}

void CGameManager::Remove_Buff(CBuff* pBuff)
{
	m_pBuff_Manager->Remove_Buff(pBuff);
}

#pragma endregion

#pragma region DAMAGEFONT_MANAGER
void CGameManager::Add_DamageFont(DAMAGEFONT eType, _float fDamage, _float3& vPostion)
{
	m_pDamageFont_Manager->Add_DamageFont(eType, fDamage, vPostion);
}

void CGameManager::Update_DamageFont(_float fTimeDelta)
{
	m_pDamageFont_Manager->Update_DamageFont(fTimeDelta);
}
#pragma endregion

#pragma region CLASH_MANAGER
_uint CGameManager::Update_Clash(_float fTimeDelta)
{
	return m_pClash_Manager->Update_Clash(fTimeDelta);
}
_bool  CGameManager::isFinshed_Clash()
{
	return m_pClash_Manager->isFinshed_Clash();
}
#pragma endregion

#pragma region DIALOGUE_CONTROLLER

void CGameManager::Start_Dialogue(_uint iNpcID, _fvector vNpcPosition)
{
	m_pDialogue_Controller->Start_Dialogue(iNpcID, vNpcPosition);
}

void CGameManager::End_Dialogue()
{
	m_pDialogue_Controller->End_Dialogue();
}

#pragma endregion

void CGameManager::Bind_PickingPos(_float3* pPickingPos)
{
	//if (nullptr == m_pPickingPos)
	//	m_pPickingPos = pPickingPos;
	//if(m_pPickingPos->y <= pPickingPos->y)
	//	m_pPickingPos = pPickingPos;
}

_vector CGameManager::Picking_Terrains()
{
	_vector vCameraPosition = XMLoadFloat4(m_pGameInstance->Get_Camera_Position());
	_float distance = { 100.f };

	const list<CGameObject*>& Terrains = m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(m_eCurLevel), TEXT("Layer_Terrain"));

	if (Terrains.empty())
	{
		m_bPicked = false;
		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	for (auto iter = Terrains.begin(); iter != Terrains.end(); iter++)
	{
		_float3 vPosition = {};

		if (dynamic_cast<CTerrain*>(*iter)->Picking(&vPosition))
		{
			_float fDist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPosition) - vCameraPosition));
			if (distance >= fDist)
			{
				distance = fDist;
				m_PickingPos = vPosition;
			}
		}
	}

	m_PickingPos;

	return XMVectorSetW(XMLoadFloat3(&m_PickingPos), 1.f);
}

void CGameManager::Free()
{
	__super::Free();

}
