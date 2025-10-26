#pragma once
#include "Client_Defines.h"

#include "GameObject.h"
#include "GameManager.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CTexture;
NS_END

NS_BEGIN(Client)

class CEffect_Manager : public CGameObject
{
private:
	CEffect_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Manager(const CEffect_Manager& Prototype);
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void Add_Effects(EFFECT eType, _uint iEffectID, const _float4x4* pWorldMatirx, CHARACTER eCharacter);
	void Start_ScreenBlur(_float fTime);
	const vector<EFFECT_EVENT_DESC>& Get_EffectTrack(CHARACTER eType, _uint iTrackIndex);
	const vector<CAMERA_SHAKE_EVENT_DESC>& Get_Camera_Track(_uint iTrackIndex);
	const vector<BLUR_EVENT_DESC>& Get_BlurTrack(_uint iTrackIndex);
	HRESULT Load_Mesh_Data(const _char* pFilePath, CHARACTER eType);
	HRESULT Load_Ground_Data(const _char* pFilePath, CHARACTER eType);
	HRESULT Load_EffectTrack(CHARACTER eType, const _char* pFilePath);

#ifdef _DEBUG
public:
	HRESULT Save_Effect(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID);
	HRESULT Save_Mesh(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID);
	HRESULT Save_Ground(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID);
#endif // _DEBUG


private:
	_bool			m_isScreenBlur = {};
	_float2			m_vScreenBlurTime = {};

	CTexture*		m_pTextureCom = { nullptr };
	CTexture*		m_pMaskTextureCom = { nullptr };
	CTexture*		m_pNoiseTextureCom = { nullptr };

	vector<vector<EFFECT_EVENT_DESC>>			m_EffectEvents[ENUM_TO_INT(CHARACTER::END)];
	vector<vector<CAMERA_SHAKE_EVENT_DESC>>		m_CameraShakeEvents;
	vector<vector<BLUR_EVENT_DESC>>				m_BlurEvents;

	vector<EFFECT_GROUND_DESC>		m_GroundEffectData[ENUM_TO_INT(CHARACTER::END)];
	vector<EFFECT_MESH_DESC>		m_MeshEffect_Datas[ENUM_TO_INT(CHARACTER::END)];

	deque<class CEffect*>			m_GroundEffects;
	deque<class CEffect*>			m_MeshEffects;

	list<class CEffect*>			m_pActiveGroundEffects;
	list<class CEffect*>			m_pActiveMeshEffects;

private:
	HRESULT Add_Components();

public:
	static CEffect_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END