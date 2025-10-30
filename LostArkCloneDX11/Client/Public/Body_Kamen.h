#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
NS_END

NS_BEGIN(Client)

class CBody_Kamen final : public CPartObject
{
public:
	typedef struct BodyKamen_Desc : public CPartObject::PARTOBJECT_DESC
	{
		PHASE* pPhase = {};
	}BODYKAMEN_DESC;

private:
	CBody_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Kamen(const CBody_Kamen& Prototype);
	virtual ~CBody_Kamen() = default;

public:
	const _bool isAnimationFinish() const {
		return m_isAnimationFinish;
	}
	_float				Get_TrackPoisiton();
	const _float4x4*	Get_BoneMatrixPtr(const _char* pBoneName) const;
	void				Set_Animation(_uint iAnimationIdex, _bool bLoop = false, _float fLerpTime = 0.2f);
	void				Change_Model(PHASE eType);

#ifdef _DEBUG
	void Set_Trigge() { m_isTrigge = false; }
#endif // _DEBUG


public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual HRESULT		Render_Shadow() override;
	void				Trigger_RimLight(_uint iColorIndex, _float fDecreasePerSec = 0.5f);

private:
	_bool					m_isAnimationFinish = {};
	_bool					m_bApplyRimLight = {};

	_uint					m_iPassIndex = {};
	_uint					m_iNumMesh = {};
	_uint					m_iCurModelIndex = {};
	_uint					m_iRimLightColorIndex = {};

	_int					m_iAnimIndex = {};
	_float4					m_vBaseColor = {};
	_float4					m_vClearColor = {};

	_float					m_fDecreasePerSec = {};
	_float					m_fRimStrength = {};
	_float					m_fRimPower = {};
	_float4					m_vRimColor[2] = {};

#pragma region TESTCODE
	_float3 m_vPos = {};
#pragma endregion
	PHASE*					m_pPhase = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CTexture*				m_pEmssiveTextureCom = { nullptr };

	vector<CModel*>			m_pModelComs;

	const _float4x4*		m_pCameraTargetBoneMatrix = {};
	_float4x4				m_CameraTargetBoneWorldMatrix = {};

#ifdef _DEBUG

	_bool					m_isDebug = {};
	_float					m_fKeyFrame = {};
	_uint					m_iEffectID = {};

	list<EFFECT_EVENT_DESC>	m_EffectEvents;

	_bool					m_isTrigge = {};
	_int					m_currentIndex = {};
	vector<string>			m_SoundNames;
	_float					m_fVolume = { 1.f };
	_float					m_fTestSoundKey = {};
#endif // _DEBUG

private:
	HRESULT			Add_Components();
	HRESULT			Bind_ShaderResources();

public:
	static CBody_Kamen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
