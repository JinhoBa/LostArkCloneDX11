#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"
#include "GameManager.h"

#pragma region UI
#include "HUD.h"
#include "Canvars.h"
#include "Background_Logo.h"
#include "LogoPanel.h"
#include "ServerListPanel.h"
#include "ExpBar.h"
#include "HUD_Panel.h"
#include "HpBar.h"
#include "MpBar.h"
#include "Identity.h"
#include "Iden_Gauge.h"
#include "WeaponUI.h"
#include "ActiveSlot.h"
#include "SkillSlot.h"
#include "AwakeSlot.h"
#include "TopMenu.h"
#include "UtilitySkillUI.h"
#include "UtilitySkillSlot.h"
#include "HoldingSkillUI.h"
#include "BuffUI.h"
#include "BossUI.h"
#include "BossHpBar.h"
#include "BossHpFrame.h"
#include "BossEnterUI.h"
#include "ClashUI.h"
#include "DialogueUI.h"
#pragma endregion

#pragma region GAMEOBJECT
#include "Terrain.h"
#include "Collider.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"
#include "HpBar_Player.h"
#include "Test_Effect.h"
#include "TestMeshEffect.h"
#include "Effect_Ground.h"
#include "Effect_Trail.h"
#include "Effect_Mesh.h"
#include "Effect_Manager.h"

#include "Body_Monster.h"
#include "HpBar_Monster.h"
#include "Monster_Named.h"
#include "Kamen.h"
#include "Body_Kamen.h"
#include "Weapon_Kamen.h"
#include "MapObject.h"
#include "SkySphere.h"
#include "Dynamic_SkyBox.h"
#include "Kamen_Sword.h"
#include "Kamen_Area.h"
#include "Npc.h"
#pragma endregion

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }, m_pContext{pContext},
	m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);
	
	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;
	m_fLoadProgress = 1.f;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	// 서브 스레드에서 사용할 COM 객체 초기화 작업
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	case LEVEL::TUTORIAL:
		hr = Loading_For_Tutorial();
		break;

	case LEVEL::BOSS:
		hr = Loading_For_Boss();
		break;

	case LEVEL::MAP_EDITOR:
		hr = Loading_For_MapEditor();
		break;
	}

	LeaveCriticalSection(&m_Critical_Section);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

_float CLoader::Output()
{
	SetWindowText(g_hWnd, m_strMessage.c_str());

	return m_fLoadProgress;
}

HRESULT CLoader::Loading_For_Logo()
{
	m_fLoadProgress = 10.f;
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_LogoBackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LogoBackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/Logo_frames/Logo%03d.png"), 150))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/Logo.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ServerListBack */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_ServerListBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/SelectServer.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ExitButton */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_ExitButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Button/Exit_Button%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ExitButton */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_EmptyButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Button/EmptyButton%d.dds"), 2))))
		return E_FAIL;

	

#pragma endregion
	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_fLoadProgress = 40.f;
	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_fLoadProgress = 60.f;
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	m_fLoadProgress = 80.f;

#pragma region GAEMOBJCET_PROTOTYPE
	
#pragma endregion

#pragma region UI_PROTOTYPE
	/* For.Prototype_GameObject_Background_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_Background_Logo"),
		CBackground_Logo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LogoPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoPanel"),
		CLogoPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ServerListPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_ServerListPanel"),
		CServerListPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	m_fLoadProgress = 100.f;

	m_strMessage = TEXT("로딩이 완료되었습니다..");

	
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	m_fLoadProgress = 1.f;
	m_strMessage = TEXT("데이터 파일을 로딩 중 입니다.");

	if(FAILED(CGameManager::GetInstance()->Load_AnimationData("../Bin/Resources/Data/Monster/Monster_Animation_Data.xml")))
		return E_FAIL;

	if(FAILED(CGameManager::GetInstance()->Load_Monster_SkillData("../Bin/Resources/Data/Monster/Monster_Skill_Data.xml")))
		return E_FAIL;

	if (FAILED(CGameManager::GetInstance()->Load_CameraAnimation("../Bin/Resources/Data/CameraAnimation/CameraAnimation.xml")))
		return E_FAIL;
	
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE
	/* For.Prototype_Component_Texture_KamenBase */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenBase"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Kamen/Texture2D/mn_cdkcn_00-1_d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KamenEmssive */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KamenEmssive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Kamen/Texture2D/mn_cdkcn_00-1_e.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WorldHpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_WorldHpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/WorldHp/WorldHpBar_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/fx_base_0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_Trail */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/fx_base_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_decal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_decal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/fx_base_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_hit */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_hit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/fx_base_%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/fx_base_%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mask/fx_mask_%d.dds"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_Noise */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/fx_noise_%d.dds"), 7))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_NoiseFolder */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_NoiseFolder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_DiffuseFolder */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_DiffuseFolder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TestEffect_MaskFolder */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TestEffect_MaskFolder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mask")))))
		return E_FAIL;

#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ExpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/EXP/ExpBar%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/HpBar/Hpbar%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MpBar/Mpbar%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_quickslot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_quickslot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/QuickSlot/quickslot_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Iden_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Iden/identity_weaponFrame.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Iden_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Weapon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Iden/identity_weapon%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TopMenu */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_TopMenu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TopMenu/topmenu.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Skill/skill_%d.dds"), 14))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_EpicSkill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_EpicSkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Skill/EpicSkill_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UtilitySkill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_UtilitySkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Skill/UtilitySkill_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Buff */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Buff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Buff/buff_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BuffFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_BuffFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Buff/BuffFrame.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Iden_Gauge */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/iden/identity_Gauge_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Iden_Gauge_Fire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Gauge_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/iden/Gauge/identity_Gauge_Fire_%d.dds"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Iden_HoldingSkillUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_HoldingSkillUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/HoldingSkill/Holding_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Message_Background */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Message_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TopMenu/Message_Background.dds"), 1))))
		return E_FAIL;

#pragma endregion

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");
#pragma region MODEL
	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(-90.f));
	///* Player_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Player/Player.bin", PreTransformMatrix))))
		return E_FAIL;

	/* Lance_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player_Lance"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Player_Weapons/WP_L.fbx"))))
		return E_FAIL;

	/* Spear_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player_Spear"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Player_Weapons/WP_S.fbx"))))
		return E_FAIL;

	///* Monster1_Model */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Monster1"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Monster1/Monster1.fbx"))))
	//	return E_FAIL;

	///* Monster2_Model */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Monster2"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Monster2/Monster2.fbx"))))
	//	return E_FAIL;

	///* Kamen */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Kamen/Kamen.fbx"))))
	//	return E_FAIL;

	///* Kamen_Horse */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_Horse"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Kamen_Horse/Kamen_with_Horse.fbx"))))
	//	return E_FAIL;

	///* Kamen_3 */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_3"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Kamen3/Kamen3.fbx"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Weapon_Kamen"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Kamen_Weapon/KamenWeapon.fbx"))))
	//	return E_FAIL;

	/* TEST CODE */
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
		CModel::Create_BinaryFile(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Player/Player.fbx", PreTransformMatrix))))
		return E_FAIL;*/
#pragma endregion

#pragma region EFFECT_MESH
	/*For Prototype_Component_Model_Auro*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Auro"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_auro_01.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Circle1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Circle1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_circle_01.fbx"))))
		return E_FAIL;
#pragma region CYLINDER
	/*For Prototype_Component_Model_Cylinder1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_cylinder_01.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Cylinder2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_cylinder_02.fbx"))))
		return E_FAIL;
#pragma endregion

	
#pragma region DECAL
	/*For Prototype_Component_Model_Decal_1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_decal_01.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Decal_2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_decal_02.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Decal_3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Decal_3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_decal_03.fbx"))))
		return E_FAIL;
#pragma endregion

	

	/*For Prototype_Component_Model_Dome*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Dome"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_dome.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Line"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_line_01.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Potal*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Potal"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_potal_01.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Ring*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Ring"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_ring_01.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Screw*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Screw"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_screw_01.fbx"))))
		return E_FAIL;

	/*For Prototype_Component_Model_Screwfront*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Screwfront"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_screwfront_01.fbx"))))
		return E_FAIL;
#pragma region TRAIL
	/*For Prototype_Component_Model_Trail1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_trail_01.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Trail2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_trail_02.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Trail3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_trail_03.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Trail4*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail4"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_trail_04.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Trail5*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trail5"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_trail_05.fbx"))))
		return E_FAIL;
	/*For Prototype_Component_Model_Cylinder3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Cylinder3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::MESH, "../Bin/Resources/EffectMesh/fm_cylinder_03.fbx"))))
		return E_FAIL;
	
#pragma endregion

	



#pragma endregion


	

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");
#pragma region SHADER
	/* For.Prototype_Component_Shader_VertexMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTXMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElement))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VertexMeshEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMeshEffect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTXMeshEffect.hlsl"), VTXMESH::Elements, VTXMESH::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPointParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointParticle.hlsl"), VTX_POS_INSTANCE_PARTICLE::Elements, VTX_POS_INSTANCE_PARTICLE::iNumElement))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxLineTrail */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxLineTrail"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxLineTrail.hlsl"), VTX_POS_INSTANCE_LINE::Elements, VTX_POS_INSTANCE_LINE::iNumElement))))
		return E_FAIL;
#pragma endregion


	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

#pragma region COMPONENT
	/*For Prototype_Component_StateMachine*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_Component_Navigation*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Trision"),
		CNavigation::Create(m_pDevice, m_pContext, "../Bin/Resources/Data/Navigtion/Trision_Navigation.bin"))))
		return E_FAIL;

	/*For Prototype_Component_Navigation_KamenPhase2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_KamenPhase2"),
		CNavigation::Create(m_pDevice, m_pContext, "../Bin/Resources/Data/Navigtion/Kamen2_Navigation.bin"))))
		return E_FAIL;

	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC Point_Desc = {};
	Point_Desc.iNumInstance = 1000;
	Point_Desc.vCenter = _float3(0.f, 2.f, 0.f);
	Point_Desc.vRange = _float3(3.f, 3.f, 3.f);
	Point_Desc.vSize = _float2(0.1f, 0.5f);
	Point_Desc.isLoop = true;
	Point_Desc.vLifeTime = _float2(1.0f, 10.f);
	Point_Desc.vSpeed = _float2(1.f, 3.f);

	/*For Prototype_Component_VIBuffer_Point_Instance_TestEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_TestEffect"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &Point_Desc))))
		return E_FAIL;

	Point_Desc.iNumInstance = 1;
	Point_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	Point_Desc.vRange = _float3(0.f, 0.f, 0.f);
	Point_Desc.vSize = _float2(1.f, 1.f);
	Point_Desc.isLoop = true;
	Point_Desc.vLifeTime = _float2(1.0f, 1.f);
	Point_Desc.vSpeed = _float2(5.f, 5.f);


	/*For Prototype_Component_VIBuffer_Point_Instance_TestEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Point_Instance_GroundEffect"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &Point_Desc))))
		return E_FAIL;

	CVIBuffer_Line_Instance::LINE_INSTANCE_DESC Line_Desc = {};
	Line_Desc.iNumInstance = 1000;
	Line_Desc.vCenter = _float3(0.f, 2.f, 0.f);
	Line_Desc.vRange = _float3(3.f, 3.f, 3.f);
	Line_Desc.vSize = _float2(0.1f, 0.5f);
	Line_Desc.isLoop = false;
	Line_Desc.vLifeTime = _float2(1.0f, 1.f);
	Line_Desc.vWidth = _float3(1.f, 5.f, 5.f);

	/*For Prototype_Component_VIBuffer_Point_Instance_TestEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Line_Instance_TrailEffect"),
		CVIBuffer_Line_Instance::Create(m_pDevice, m_pContext, &Line_Desc))))
		return E_FAIL;

	/*For Prototype_Component_Collider_AABB*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	/*For Prototype_Component_Collider_OBB*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	/*For Prototype_Component_Collider_Sphere*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_Trision_Floor*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_Trision_Floor"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, "../Bin/Resources/HeightMap/Terrision_Floor.bin", 50, 50))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_Trision_Stair*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_Trision_Stair"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr, 4, 6))))
		return E_FAIL;


	/*For Prototype_Component_VIBuffer_Terrain_80*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_80"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr, 80, 80))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_64*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_64"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr, 64, 64))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_45*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_45"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr , 45, 45))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_32*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_32"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr, 32, 32))))
		return E_FAIL;

	/*For Prototype_Component_VIBuffer_Terrain_16*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain_16"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, nullptr, 16, 16))))
		return E_FAIL;


#pragma endregion
	
#pragma region GAEMOBJCET_PROTOTYPE
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Weapon_Player"),
		CWeapon_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HpBar_Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpBar_Player"),
		CHpBar_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Test_Effect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Test_Effect"),
		CTest_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Test_MeshEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Test_MeshEffect"),
		CTestMeshEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Ground*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Ground"),
		CEffect_Ground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Mesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Mesh"),
		CEffect_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Trail*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Trail"),
		CEffect_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster_Named */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster_Named"),
		CMonster_Named::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster"),
		CBody_Monster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HpBar_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpBar_Monster"),
		CHpBar_Monster::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Kamen */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Kamen"),
		CKamen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Kamen */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Kamen"),
		CBody_Kamen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Kamen */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Weapon_Kamen"),
		CWeapon_Kamen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Manager */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect_Manager"),
		CEffect_Manager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region UI_PROTOTYPE
	/* For.Prototype_GameObject_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
		CExpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
		CHUD_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hpbar"),
		CHpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Mpbar"),
		CMpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Identity */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Identity"),
		CIdentity::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WeaponUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WeaponUI"),
		CWeaponUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AwakeSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AwakeSlot"),
		CAwakeSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ActiveSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ActiveSlot"),
		CActiveSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkillSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkillSlot"),
		CSkillSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TopMenu */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TopMenu"),
		CTopMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Iden_Gauge */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Iden_Gauge"),
		CIden_Gauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UtilitySkillUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UtilitySkillUI"),
		CUtilitySkillUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UtilitySkillSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UtilitySkillSlot"),
		CUtilitySkillSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HoldingSkillUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HoldingSkillUI"),
		CHoldingSkillUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BuffUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BuffUI"),
		CBuffUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


	return S_OK;
}

HRESULT CLoader::Loading_For_Tutorial()
{
	if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/MapData/Trision.xml")))
		return E_FAIL;

	if (FAILED(Loading_For_GamePlay()))
		return E_FAIL;
	
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_Dialogue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Texture_Dialogue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialogue/Dialogue_%d.dds"), 2))))
		return E_FAIL;


#pragma endregion
	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(-90.f));

	/* For.Prototype_Component_Model_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_Sky.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Npc */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Npc"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Npc/Npc.fbx"))))
		return E_FAIL;

#pragma region TRISION
	/* For.Prototype_Component_Model_Trision */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision2 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_01b.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Botton01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Botton01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom01a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Botton02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Botton03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Wall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Wall"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_wall.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_arkprop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_arkprop"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_arkprop.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_circle01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_circle01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_circle01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_circle02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_circle02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_circle02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Object01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Object01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stone01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stone02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stone03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_2c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_2c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision01_c.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01b.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01c.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01d */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01d"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01d.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01e.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01f */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_Component_Model_Trision_Stair01f"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01f.fbx"))))
		return E_FAIL;



#pragma endregion

	m_fLoadProgress = 40.f;
	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_fLoadProgress = 60.f;
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	m_fLoadProgress = 80.f;

#pragma region GAEMOBJCET_PROTOTYPE
	/* For.Prototype_GameObject_Npc */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_Npc"),
		CNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MapObjcet */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkySphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_SkySphere"),
		CSkySphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region UI_PROTOTYPE
	/* For.Prototype_GameObject_DialogueUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_DialogueUI"),
		CDialogueUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
	m_fLoadProgress = 100.f;

	m_strMessage = TEXT("로딩이 완료되었습니다..");


	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss()
{

	m_strMessage = TEXT("데이터를(을) 로딩 중 입니다.");
	if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/MapData/Kamen.xml")))
		return E_FAIL;

	if (FAILED(CGameManager::GetInstance()->Load_KamenData("../Bin/Resources/Data/Kamen/Kamen_Data.xml")))
		return E_FAIL;



	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE
	/* For.Prototype_Component_Texture_skybox_eclipse */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_skybox_eclipse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/SkyBox/Texture2D/lv_skybox_eclipse_01_d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sword_Kamen_Diffuse */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Sword_Kamen_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Kamen_Sword/Texture2D/wp_mn_cdkcn_01_d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sword_Kamen_Emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Sword_Kamen_Emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Kamen_Sword/Texture2D/wp_mn_cdkcn_01_e.dds"), 1))))
		return E_FAIL;
	
#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_BossHpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_BossHpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BossHp/BossHp_%d.dds"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BossHpBarFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_BossHpBarFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BossHp/BossHpFrame.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clash_Circle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Clash_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Clash/clash_circle_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clash_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Clash_Key"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Clash/clash_key_%d.dds"), 5))))
		return E_FAIL;
#pragma endregion

	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Model_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_DynamicSky"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/SkyBox/Sky_Model.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_Sword"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Kamen_Sword/Kamen_Sword.fbx"))))
		return E_FAIL;

	_matrix			PreTransformMatrix = XMMatrixIdentity();
#pragma region KAMEN_MAP
	/* For.Prototype_Component_Model_Kamen_chair01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_chair01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_chair01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_chair01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01b.fbx"))))
		return E_FAIL;
	m_fLoadProgress = 40.f;
#pragma region FLOOR

	/* For.Prototype_Component_Model_Kamen_floar01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar04 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor04.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05b.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05c.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05d */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05d"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05d.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05e.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05f */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05f"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05f.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05g */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05g"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05g.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05h */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar05h"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05h.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar06 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor06.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar07 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar07"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor07.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar08"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar08b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08b.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar08e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08e.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar09 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar09"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor09.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar10 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar10"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor10.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar11 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar11"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor11.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar11a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar11a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor11a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar12 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar12"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor12.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar12a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_floar12a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor12a.fbx"))))
		return E_FAIL;


#pragma endregion

#pragma region DECO
	/* For.Prototype_Component_Model_Kamen_deco01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco02c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02c.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco02a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco02b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02b.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco04 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco04.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco05 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco05.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco06 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_deco06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco06.fbx"))))
		return E_FAIL;

#pragma endregion
	m_fLoadProgress = 60.f;
#pragma region PILLAR
	/* For.Prototype_Component_Model_Kamen_pillar01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar01a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar02a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02low */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar02low"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02low.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar04*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar04.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar05*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar05.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar06*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar06.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar07 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar07"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar07.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar08 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar08"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar08.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar09"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar09a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09low */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_pillar09low"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09low.fbx"))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_Component_Model_kamen_gate01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_Component_Model_Kamen_gate01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_gate01.fbx"))))
		return E_FAIL;
	m_fLoadProgress = 80.f;

#pragma endregion

#pragma endregion

	
	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	/*For Prototype_Component_Navigation*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation_Kamen"),
		CNavigation::Create(m_pDevice, m_pContext, "../Bin/Resources/Data/Navigtion/Kamen_Navigation.bin"))))
		return E_FAIL;

	
#pragma region GAEMOBJCET
	/*For Prototype_GameObject_Dynamic_SkyBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Dynamic_SkyBox"),
		CDynamic_SkyBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_Kamen_Sword*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Kamen_Sword"),
		CKamen_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_Kamen_Sword*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_Kamen_Area"),
		CKamen_Area::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region UI
	/*For Prototype_GameObject_BossUI*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossUI"),
		CBossUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_BossHpBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossHpBar"),
		CBossHpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_BossHpFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossHpFrame"),
		CBossHpFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_BossEnterUI*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_BossEnterUI"),
		CBossEnterUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For Prototype_GameObject_ClashUI*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_ClashUI"),
		CClashUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	m_fLoadProgress = 100.f;

	m_strMessage = TEXT("로딩이 완료되었습니다..");


	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEditor()
{
	if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/MapData/Kamen_Phase2.xml")))
		return E_FAIL;

	/*if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/MapData/Kamen.xml")))
		return E_FAIL;*/

	if (FAILED(Loading_For_GamePlay()))
		return E_FAIL;

	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");

	///* For.Prototype_Component_Texture_LogoBackGround */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Prototype_Component_TextureMap_MeshPreview"),
	//	CTextureMap::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Map/Kamen/"), CGameManager::GetInstance()->Get_PreviewTexturesPtr()))))
	//	return E_FAIL;

#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE

#pragma endregion
	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Model_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_Sky.bin"))))
		return E_FAIL;

#pragma region MAP_MODEL

#pragma region KAMEN
	/* For.Prototype_Component_Model_Kamen_chair01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_chair01b.bin"))))
		return E_FAIL;

#pragma region FLOOR

	/* For.Prototype_Component_Model_Kamen_floar01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor03.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar04 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor04.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05b.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05c.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05d */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05d"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05d.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05e.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05f */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05f"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05f.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05g */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05g"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05g.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05h */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05h"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor05h.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar06 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor06.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar07 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar07"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor07.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar08"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar08b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08b.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar08e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar08e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor08e.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar09 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar09"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor09.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar10 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar10"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor10.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar11 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar11"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor11.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar11a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar11a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor11a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar12 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar12"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor12.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar12a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar12a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_floor12a.bin"))))
		return E_FAIL;


#pragma endregion

#pragma region DECO
	/* For.Prototype_Component_Model_Kamen_deco01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco02c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02c.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco02a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco02b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco02b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco02b.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco03.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco04 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco04.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco05 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco05.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_deco06 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_deco06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_deco06.bin"))))
		return E_FAIL;

#pragma endregion



#pragma region PILLAR
	/* For.Prototype_Component_Model_Kamen_pillar01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar01a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar02a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar02low */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar02low"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar02low.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar03.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar04*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar04"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar04.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar05*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar05"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar05.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar06*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar06"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar06.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar07 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar07"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar07.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar08 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar08"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar08.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar09"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar09a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_pillar09low */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_pillar09low"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_pillar09low.bin"))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_Component_Model_kamen_gate01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_gate01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Kamen/kamen_gate01.bin"))))
		return E_FAIL;


#pragma endregion

	
#pragma region TRISION
	/* For.Prototype_Component_Model_Trision */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision2 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_01b.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Botton01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Botton01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom01a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Botton02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Botton03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Botton03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_bottom03.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Wall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Wall"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_wall.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_arkprop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_arkprop"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision_arkprop.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_circle01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_circle01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_circle01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_circle02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_circle02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_circle02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Object01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Object01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stone01"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone01.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stone02"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone02.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stone03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stone03"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stone03.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_2c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_2c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/Trision01_c.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01a"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01a.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01b"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01b.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01c"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01c.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01d */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01d"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01d.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01e"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01e.bin"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Trision_Stair01f */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Trision_Stair01f"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Map/Trision/trision_stair01f.bin"))))
		return E_FAIL;

	

#pragma endregion

#pragma endregion


	


	m_fLoadProgress = 40.f;
	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_fLoadProgress = 60.f;
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	/* For.Prototype_GameObject_MapObjcet */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkySphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Prototype_GameObject_SkySphere"),
		CSkySphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;





	m_fLoadProgress = 80.f;
#pragma region GAEMOBJCET_PROTOTYPE

#pragma endregion

#pragma region UI_PROTOTYPE


#pragma endregion
	m_fLoadProgress = 100.f;

	m_strMessage = TEXT("로딩이 완료되었습니다..");


	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Create : CLoader");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
