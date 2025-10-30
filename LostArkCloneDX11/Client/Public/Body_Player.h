#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CStateMachine;
NS_END

NS_BEGIN(Client)

class CBody_Player final : public CPartObject
{
public:
	typedef struct BodyPlayer_Desc : public CPartObject::PARTOBJECT_DESC
	{
		_float* pAttackSpeed;
		STANCE* pStance;
	}BODYPLAYER_DESC;

private:
	CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Player(const CBody_Player& Prototype);
	virtual ~CBody_Player() = default;

public:
	const _bool isAnimationFinish() const { 
		return m_isAnimationFinish; }
	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;
	_float Get_TrackPoisiton();
	void Set_Animation(_uint iAnimationIdex, _bool bLoop = false, _float fLerpTime = 0.1f);
	void Toggle_RimLight();

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual HRESULT		Render_Shadow() override;


private:
	_bool					m_isAnimationFinish = {};
	_bool					m_bApplyRimLight = {};

	_int					m_iAnimIndex = {};
	_uint					m_iNumMesh = {};

	STANCE*					m_pStance = { nullptr };
	_float*					m_pAttackSpeed = {nullptr};

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

	const _float4x4*		m_pCameraTargetBoneMatrix = {};
	_float4x4				m_CameraTargetBoneWorldMatrix = {};

	_float					m_fRimStrength = {};
	_float					m_fRimPower = {};
	_float4					m_vRimColor[ENUM_TO_INT(STANCE::IDEN_END)] = {};

#ifdef _DEBUG
	_bool					m_isDebug = {};
	_float					m_fKeyFrame = {};

	LIGHT_DESC				LightDesc = {};
	_float					m_fRange = {};
	_float3					m_vPosition = {};
#endif // _DEBUG


private:
	HRESULT			Add_Components();
	HRESULT			Bind_CameraBoneMatrix();

public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
