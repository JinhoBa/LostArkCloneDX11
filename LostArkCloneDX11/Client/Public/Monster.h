#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"

#include "Enemy.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CCollider;
NS_END

NS_BEGIN(Client)
class CMonster abstract : public CEnemy
{
public:
	enum STATE {IDLE, ATTACK, TURN, RUN, HIT, DEAD, STATE_END};
public:
	typedef struct Monster_Desc : public GAMEOBJECT_DESC
	{
		_uint iMonsterID{};
		_uint iNumAttack{};
		_float fMaxHp{};
		_float fHp{};
		_float fAttack{};
		_float fDetectDistance{};
		_float fAttackRange{};
		_float4 vPosition{};
		_wstring strModelPrototypeTag{};
	}MONSTER_DESC;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	const _bool isInAttackRange()const {
		return m_fDistToPlayer <= m_EnemyInfo.fAttackRange;
	}
	_vector Get_Position() {
		return m_pTransformCom->Get_State(Engine::STATE::POSITION);
	}
	_bool Turn(_float fTimeDelta) {
		return m_pTransformCom->TurnLerp(m_pPlayerTransformCom->Get_Position(), fTimeDelta);
	}

	HIT_TYPE	Get_HitType();
	const _bool isInBattle() const { return m_bInBattle; }
	const _bool isAnimationFinish();
	CState*		Get_State(CMonster::STATE eState) const { return m_States[ENUM_TO_INT(eState)]; }
	void		Set_Animation(ANIMATIONSLOT eAnim);
	void		Chase(_float fTimeDelta);
	

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_bool				m_isHit = {};
	_bool				m_bInBattle = {};
	_float				m_fSpeed = {};
	_uint				m_iMonsetrID = {};
	_uint				m_iNumAttack = {};

	_float				m_fDistToPlayer = {};

	MONSTER				m_eType = {};

	const _float4x4*	m_pRootBoneMatrix = { nullptr };

	CStateMachine*		m_pStateMachineCom = { nullptr };
	CState*				m_States[STATE_END] = {};
	CTransform*			m_pPlayerTransformCom = { nullptr };
	CNavigation*		m_pNavigationCom = {nullptr};
	CCollider*			m_pColliderCom = { nullptr };

protected:
	HRESULT			Ready_PartObjects(_wstring& strModelPrototypeTag);
	void			Detect_Player();

#ifdef _DEBUG
protected:
	_float m_fWeight_Sepration = {1.f};
	_float m_fWeight_Cohesion = {1.f};
#endif // _DEBUG


public:
	virtual CGameObject* Clone(void* pArg) PURE ;
	virtual void Free() override;
};

NS_END