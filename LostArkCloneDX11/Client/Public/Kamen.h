#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

NS_BEGIN(Engine)
class CStateMachine;
class CState;
class CCollider;
NS_END

NS_BEGIN(Client)

class CKamen final : public CEnemy
{
public:
	enum class KAMENSTATE {INTRO, IDLE, 
		ATTACK_NORMAL, ATTACK_SPIN, ATTACK_COMBO, ATTACK_CHARGE, ATTACK_SWORD,
		DEAD, END };

private:
	CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKamen(const CKamen& Prototype);
	virtual ~CKamen() = default;

public:
	CState* Get_State(KAMENSTATE eState) const {
		return m_States[ENUM_TO_INT(eState)];
	}
	_float			Get_TrackPositon();

	_bool	isAnimationFinish();
	void	Set_Animation(_uint iIndex, _bool bLoop = false, _float fLerpTime = 0.2f);
	void	Change_Phase(PHASE ePhase);
	void	Chase(_float fTimeDelta);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		OnHit(const ATTACK_DESC& Attack_Desc)override;

	void Update_HitBox(_uint iSkillID, _uint iHitIndex);
	

private:
#ifdef _DEBUG
	_bool isCollUpdate = { false };
	_float3 m_vHitBoxCenter = {};
	_float3 m_vHitBoxExtents = {};
#endif // _DEBUG
	PHASE				m_ePhase = { PHASE::END };

	_uint				m_iCurSkillID = {};
	_uint				m_iCurHitIndex = {};

	CStateMachine*		m_pStateMachineCom = { nullptr };
	CState*				m_States[ENUM_TO_INT(KAMENSTATE::END)] = {};

	CNavigation*		m_pNavigationCom = { nullptr };
	CTransform*			m_pPlayerTransformCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CCollider*			m_pHitBoxCom = { nullptr };

	const _float4x4*	m_pRootBoneMatrix = { nullptr };

private:
	HRESULT			Reay_Component();
	HRESULT			Reay_States();
	HRESULT			Ready_PartObjects();

public:
	static CKamen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
