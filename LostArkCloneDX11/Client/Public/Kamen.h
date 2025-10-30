#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

NS_BEGIN(Engine)
class CStateMachine;
class CState;
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)

class CKamen final : public CEnemy
{
public:
	enum class KAMENSTATE {
		INTRO, IDLE, TRUN,
		ATTACK_NORMAL, ATTACK_SPIN, ATTACK_COMBO, ATTACK_CHARGE, ATTACK_SWORD,
		CUTSCENE,CLASH, CRITICAL, MOVE, DEAD,
		END };

private:
	CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKamen(const CKamen& Prototype);
	virtual ~CKamen() = default;

public:
	_bool	isAnimationFinish();
	const ENEMY_INFO* Get_InfoPtr() { return &m_EnemyInfo; };
	CState* Get_State(KAMENSTATE eState) const { return m_States[ENUM_TO_INT(eState)]; }
	_float	Get_TrackPositon();
	CPartObject* Get_PartObject(const _tchar* PartObjectTag);
	void	Set_Animation(_uint iIndex, _bool bLoop = false, _float fLerpTime = 0.2f);
	void	Set_HitBox(_float3& vCenter, _float3& vExtends, _float3& vOrientation, COLLIDER eHitboxType = COLLIDER::OBB);
	void	Change_Phase(PHASE ePhase);
	
	_bool Turn(_float fTimeDelta) {
		return m_pTransformCom->TurnLerp(m_pPlayerTransformCom->Get_Position(), fTimeDelta * 0.7f);
	}

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		OnHit(const ATTACK_DESC& Attack_Desc)override;

	void				Update_HitBox(_uint iSkillID, _uint iHitIndex, COLLIDER eHitboxType = COLLIDER::OBB);
	_bool				MoveToPlayer(_float fTimeDelta);
	_bool				Reposition();
	void				Chase(_float fTimeDelta);
	void				HitBox_Event(_uint iSkillID, _uint m_iHitIndex);

private:
#ifdef _DEBUG
	KAMENSTATE m_eType = {};

	_bool			isCollUpdate = { false };
	_bool			isSphereUpdate = { false };
	_float3			m_vHitBoxCenter = {};
	_float3			m_vHitBoxExtents = {};
	_float3			m_vOrientation = {};
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
	CCollider*			m_pHitBoxShpereCom = { nullptr };

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
