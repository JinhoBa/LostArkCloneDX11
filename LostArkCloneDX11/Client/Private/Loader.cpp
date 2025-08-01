#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"

#pragma region UI_HEADER
#include "Canvars.h"
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

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	// ���� �����忡�� ����� COM ��ü �ʱ�ȭ �۾�
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
	}

	LeaveCriticalSection(&m_Critical_Section);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::Output()
{
	SetWindowText(g_hWnd, m_strMessage.c_str());
}

HRESULT CLoader::Loading_For_Logo()
{
	m_strMessage = TEXT("�ؽ��ĸ�(��) �ε� �� �Դϴ�.");
#pragma region GAEMOBJCET_TEXTURE

#pragma endregion

#pragma region UI_TEXTURE
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Canvars"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo_frames/Logo_frame_0%02d.png"), 50))))
		return E_FAIL;
#pragma endregion
	m_strMessage = TEXT("�𵨸�(��) �ε� �� �Դϴ�.");

	m_strMessage = TEXT("���̴���(��) �ε� �� �Դϴ�.");


	m_strMessage = TEXT("��ü������(��) �ε� �� �Դϴ�.");
#pragma region GAEMOBJCET_PROTOTYPE

#pragma endregion

#pragma region UI_PROTOTYPE
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_INT(LEVEL::LOGO), TEXT("Prototype_GameObject_Canvars"),
		CCanvars::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	
	m_strMessage = TEXT("�ε��� �Ϸ�Ǿ����ϴ�..");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
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
