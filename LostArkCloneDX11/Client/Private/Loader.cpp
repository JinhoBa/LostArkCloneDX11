#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"

#pragma region UI
#include "Canvars.h"
#include "Background_Logo.h"
#include "LogoPanel.h"
#include "ServerListPanel.h"
#include "ExpBar.h"
#include "HUD.h"
#include "HpBar.h"
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo_frames/Logo%03d.png"), 301))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ServerListBack */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_ServerListBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/SelectServer.png"), 1))))
		return E_FAIL;
#pragma endregion
	m_fLoadProgress = 20.f;
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	Sleep(1000);
	m_fLoadProgress = 40.f;
	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");
	Sleep(1000);
	m_fLoadProgress = 60.f;
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");
	Sleep(1000);
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

	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ExpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/EXP/ExpBar%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/HpBar/Hpbar%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MpBar/Mpbar%d.png"), 2))))
		return E_FAIL;

#pragma endregion

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");


	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");


	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	
#pragma region GAEMOBJCET_PROTOTYPE

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

	/* For.Prototype_GameObject_ExpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hpbar"),
		CHpBar::Create(m_pDevice, m_pContext))))
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
