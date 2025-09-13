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
	}BODYPLAYER_DESC;

private:
	CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Player(const CBody_Player& Prototype);
	virtual ~CBody_Player() = default;

public:
	const _bool isAnimationFinish() const { 
		return m_isAnimationFinish; }
	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;
	void Set_Animation(_uint iAnimationIdex, _bool bLoop = false, _float fLerpTime = 0.1f);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_bool					m_isAnimationFinish = {};
	_int					m_iAnimIndex = {};
	_uint					m_iNumMesh = {};
	_uint					m_iCameraTargetBoneIndex = {};

	_float*					m_pAttackSpeed = {nullptr};

	class CCamera_Fix*		m_pCamera = { nullptr };

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	HRESULT			Add_Components();

public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
