#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"
#include "GameManager.h"

#pragma region UI
#include "Canvars.h"
#include "Background_Logo.h"
#include "LogoPanel.h"
#include "ServerListPanel.h"
#include "ExpBar.h"
#include "HUD.h"
#include "HpBar.h"
#include "MpBar.h"
#include "Identity.h"
#include "WeaponUI.h"
#include "ActiveSlot.h"
#include "SkillSlot.h"
#include "AwakeSlot.h"
#include "TopMenu.h"
#pragma endregion

#pragma region GAMEOBJECT
#include "Terrain.h"
#include "Camera_Free.h"
#include "Player.h"
#include "MapObject.h"
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

	

	if (FAILED(CGameManager::GetInstance()->Load_SkillData("../Bin/Resources/Data/Skill_Data.xml")))
		return E_FAIL;
	

	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE
	/* For.Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Test.dds"), 1))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Player/Player_d%d.dds"), 2))))
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

	/* For.Prototype_Component_Texture_Iden_Buff */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Iden_Buff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Skill/buff.dds"), 1))))
		return E_FAIL;

#pragma endregion

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Model_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Player/Player.fbx"))))
		return E_FAIL;

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Shader_VertexMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VertexMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTXMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElement))))
		return E_FAIL;


	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

#pragma region COMPONENT

	/*For Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
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

#pragma endregion

#pragma region UI_PROTOTYPE
	/* For.Prototype_GameObject_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ExpBar"),
		CExpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HUD"),
		CHUD::Create(m_pDevice, m_pContext))))
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

#pragma endregion


	return S_OK;
}

HRESULT CLoader::Loading_For_Tutorial()
{
	if (FAILED(Loading_For_GamePlay()))
		return E_FAIL;
	
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE

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
	

#pragma endregion
	m_fLoadProgress = 100.f;

	m_strMessage = TEXT("로딩이 완료되었습니다..");


	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEditor()
{
	if (FAILED(CGameManager::GetInstance()->Load_MapData("../Bin/Resources/Data/TestMap.xml")))
		return E_FAIL;

	if (FAILED(CGameManager::GetInstance()->Load_PreviewTextures("../Bin/Resources/Data/MeshPreview.xml")))
		return E_FAIL;

	if (FAILED(Loading_For_GamePlay()))
		return E_FAIL;

	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Texture_LogoBackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::MAP_EDITOR), TEXT("Prototype_Component_TextureMap_MeshPreview"),
		CTextureMap::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Map/Kamen/"), CGameManager::GetInstance()->Get_PreviewTexturesPtr()))))
		return E_FAIL;

#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE

#pragma endregion
	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

#pragma region MAP_MODEL

#pragma region KAMEN
	/* For.Prototype_Component_Model_Kamen_chair */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_chair.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_chair01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_chair02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_chair02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_chair02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar01 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor01.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar02 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor02.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar03 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor03.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar04 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor04.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05a */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05a.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05b */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05b.fbx"))))
		return E_FAIL;
	
	/* For.Prototype_Component_Model_Kamen_floar05c */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05c.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05d */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05d.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05e */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05e"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05e.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05f */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05f"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05f.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05g */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05g"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05g.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar05h */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar05h"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor05h.fbx"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Kamen_floar06 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Kamen_floar06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Kamen/keman_floor06.fbx"))))
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
