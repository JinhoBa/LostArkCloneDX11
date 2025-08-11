#include "GameInstance.h"

#include "Prototype_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Input_Device.h"
#include "Sound_Manager.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_vWinSize = { static_cast<_float>(EngineDesc.iWinSizeX) , static_cast<_float>(EngineDesc.iWinSizeY) };

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWindowMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, EngineDesc.hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	

	m_pInput_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();

	m_pPicking->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pObject_Manager->Clear_DeadObj();

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Render();

	m_pLevel_Manager->Render();

	return S_OK;
}

void CGameInstance::Clear_Resources(_uint iLevelIndex)
{
	m_pPrototype_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Random_Normal()
{
	return static_cast<_float>(rand()) / RAND_MAX;	
}

_wstring CGameInstance::Utf8ToWstring(const char* pUtf8Str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, pUtf8Str, -1, nullptr, 0);

	_wstring strTmp(size - 1, L'\0');

	MultiByteToWideChar(CP_UTF8, 0, pUtf8Str, -1, &strTmp[0], size);

	return strTmp;
}

string CGameInstance::WstringToUtf8(_wstring& wStr)
{
	if (wStr.empty()) 
		return "";

	int iSize = WideCharToMultiByte(CP_UTF8, 0, wStr.data(), (int)wStr.size(), nullptr, 0, nullptr, nullptr);

	string str(iSize, 0);

	WideCharToMultiByte(CP_UTF8, 0, wStr.data(), (int)wStr.size(), &str[0], iSize, nullptr, nullptr);

	return str;
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	return fMin + Random_Normal() * (fMax - fMin);	
}

#pragma region GRAPHIC_DEVICE

void CGameInstance::Render_Begin(const _float4* pClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(pClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();
}

void CGameInstance::Render_End()
{
	m_pGraphic_Device->Present();
}


#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}
#pragma endregion

#pragma region INPUT

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_bool CGameInstance::Get_KeyDown(_ubyte byKeyID)
{
	return m_pInput_Device->Get_KeyDown(byKeyID);
}

_bool CGameInstance::Get_KeyUp(_ubyte byKeyID)
{
	return m_pInput_Device->Get_KeyUp(byKeyID);
}

_bool CGameInstance::Get_KeyPressing(_ubyte byKeyID)
{
	return m_pInput_Device->Get_KeyPressing(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_bool CGameInstance::Get_DIMouseDown(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseDown(eMouse);
}

_bool CGameInstance::Get_DIMouseUp(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseUp(eMouse);
}

_bool CGameInstance::Get_DIMousePressing(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMousePressing(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototype, iLevelIndex, strPrototypeTag, pArg);;
}

#pragma endregion

#pragma region OBJECT_MANAGER

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);	
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);
}
list<class CGameObject*>* CGameInstance::Get_LayerObjects(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_LayerObjects(iLevelIndex, strLayerTag);
}
#pragma endregion

#pragma region SOUND_MANAGER
void CGameInstance::Play_Sound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Play_Sound(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSound_Manager->SetChannelVolume(eID, fVolume);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region PIPELINE

void CGameInstance::Set_Transform(D3DTS eState, _fmatrix Matrix)
{
	m_pPipeLine->Set_Transform(eState, Matrix);
}

const _float4x4* CGameInstance::Get_Transfrom_Float4x4(D3DTS eState) const
{
	return m_pPipeLine->Get_Transfrom_Float4x4(eState);
}

_matrix	CGameInstance::Get_Transfrom_Matrix(D3DTS eState)
{
	return m_pPipeLine->Get_Transfrom_Matrix(eState);
}

_matrix	CGameInstance::Get_Transfrom_MatrixInverse(D3DTS eState)
{
	return m_pPipeLine->Get_Transfrom_MatrixInverse(eState);
}

const _float4* CGameInstance::Get_Camera_Position() const
{
	return m_pPipeLine->Get_Camera_Position();
}

#pragma endregion

#pragma region PICKING

void CGameInstance::Transform_ToLocalSpace(const FXMMATRIX pWorldMatrixInverse)
{
	m_pPicking->Transform_ToLocalSpace(pWorldMatrixInverse);
}

_bool CGameInstance::Picking_InWorldSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut)
{
	return m_pPicking->Picking_InWorldSpace(vPointA, vPointB, vPointC, pQut);
}

_bool CGameInstance::Picking_InLocalSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut)
{
	return m_pPicking->Picking_InLocalSpace(vPointA, vPointB, vPointC, pQut);
}

#pragma endregion


void CGameInstance::Release_Engine()
{
	DestroyInstance();

	Safe_Release(m_pPicking);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Free()
{
	__super::Free();

	
}
