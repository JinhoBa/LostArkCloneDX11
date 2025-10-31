#pragma once

#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void	Update_Engine(_float fTimeDelta);
	HRESULT Draw();
	void	Clear_Resources(_uint iLevelIndex);

	_wstring	Utf8ToWstring(const char* pStr);
	string		WstringToUtf8(_wstring& wStr);

	_float		Random_Normal();
	_float		Random(_float fMin, _float fMax);
	_float2&	Get_WinSize() { return m_vWinSize; }


#pragma region GRAPHIC_DEVICE
public:
	void Render_Begin(const _float4* pClearColor);
	void Render_End();
	
#pragma endregion

#pragma region TIMER_MANAGER
	_float	Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void	Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region INPUT
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_bool	Get_KeyDown(_ubyte byKeyID);
	_bool	Get_KeyUp(_ubyte byKeyID);
	_bool	Get_KeyPressing(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseDown(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMouseUp(MOUSEKEYSTATE eMouse);
	_bool	Get_DIMousePressing(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT			Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*	Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	CComponent*	 Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	HRESULT		 Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	void		 Update_Level(_uint iLevelIndex, _float fTimeDelta);
	HRESULT		 Clear_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);
	const list<class CGameObject*>&	Get_LayerObjects(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region SOUND_MANAGER
	void Play_Sound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
#ifdef _DEBUG
	const map<TCHAR*, FMOD::Sound*>& Get_SoundMap();
#endif // _DEBUG


#pragma endregion

#pragma region RENDERER
	D3D11_VIEWPORT& Get_Veiwport();
	void		Toggle_VisibleUI();
	HRESULT		Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void		BlurBackBuffer(_float fStrength);
	HRESULT		Add_DebugComponent(CComponent* pDebugCom);
#pragma endregion

#pragma region PIPELINE
	void				Set_Transform(D3DTS eState, _fmatrix Matrix);
	const _float4x4*	Get_Transform_Float4x4(D3DTS eState);
	const _float4x4*	Get_Transform_Float4x4_Inverse(D3DTS eState);
	_matrix				Get_Transform_Matrix(D3DTS eState);
	_matrix				Get_Transform_MatrixInverse(D3DTS eState);
	const _float4*		Get_Camera_Position() const;
	const _float4*		Get_Camera_Look() const;
#pragma endregion

#pragma region PICKING
	POINT&		Get_MousePoint();
	_float3&	Get_MousePosition(RAY eSpace);

	void		Transform_ToLocalSpace(const FXMMATRIX pWorldMatrixInverse);
	_bool		Picking_InWorldSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut);
	_bool		Picking_InLocalSpace(const FXMVECTOR vPointA, const FXMVECTOR vPointB, const FXMVECTOR vPointC, _float3* pQut);
#pragma endregion

#pragma region FONT_MANAGER
	void		Add_Font(const _wstring& strFontTag, class CFont* pFont);
	void		Add_FontDesc(const _wstring& strFontTag, FONT_DESC* pFontDesc);
	void        Clear_Fonts();
#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC*	Get_LightDesc(const _tchar* pLightTag);
	void				ToggleLight(const _tchar* pLightTag, _bool bEnable);
	void				Update_Light_Position(const _tchar* pLightTag, _float3* pPosition);
	void				Update_Light_Range(const _tchar* pLightTag, _float fRange);
	void				Update_Light_Color(const _tchar* pLightTag, _uint iColorType, _float4* pColor);
	HRESULT				Add_Light(const _tchar* pLightTag, const LIGHT_DESC& LightDesc);
	HRESULT				Delete_Light(const _tchar* pLightTag);
	HRESULT				Render_Lights(class CShader* pShader, class CVIBuffer* pVIBuffer);
#pragma endregion

#pragma region COLLIDER_MANGER
	_bool	Check_Collider(class CCollider* pColldier, const _tchar* pDestLayerTag);
	void	Add_Collider(const _tchar* pLayerTag, class CCollider* pCollider);
	_vector  ComputePenetration(class CCollider* pColldier, const _tchar* pSrcLayerTag);
#pragma endregion

#pragma region CAMERA_MANAGER
	class CCamera* Find_Camera(const _wstring& strCameraNameTag);
	_float* Get_Far();
	HRESULT Add_Camera(const _wstring& strCameraNameTag, class CCamera* pGameObject);
	HRESULT Bind_Camera(const _wstring& strCameraNameTag, _bool isReturn = false, _float fLerpTime = 0.f);
#pragma endregion

#pragma region SHADOW
	HRESULT Ready_Shadow_Light(const SHADOW_LIGHT_DESC& Desc);
	HRESULT Bind_Shadow_Resource(class CShader* pShader, const _char* pContantName, D3DTS eType) const;
#pragma endregion


#pragma region RENDERTARGET_MANAGER
public:
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isClear = true);
	HRESULT End_MRT();
	HRESULT Bind_RenderTarget(const _wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
public:
	HRESULT Ready_RenderTarget_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RenderTarget_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG
#pragma endregion


private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*		m_pPrototype_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CSound_Manager*			m_pSound_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPicking*					m_pPicking = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CCollider_Manager*		m_pCollider_Manager = { nullptr };
	class CCamera_Manager*			m_pCamera_Manager = { nullptr };
	class CRenderTarget_Manager*	m_pRenderTarget_Manager = { nullptr };
	class CShadow*					m_pShadow = { nullptr };

	_float2							m_vWinSize = {};


	
public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END