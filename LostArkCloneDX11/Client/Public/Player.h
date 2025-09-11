#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"

#include "Character.h"

NS_BEGIN(Engine)
class CStateMachine;
class CState;
class CNavigation;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CCharacter
{
public:
	enum STATE {IDLE, MOVE, NORMAL_SKILL, CHARGE_SKILL, COMBO_SKILL, DASH, CHANGE_STANCE, HIT, STATE_END};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	const STANCE	Get_Stance() const { return m_PlayerInfo.eStance; }
	PLAYER_INFO*	Get_Info() { return &m_PlayerInfo; }
	const list<class CBuff*>& Get_BuffList() { return m_Buffs; }
	const CHARGE_SKILL_DESC* Get_ChargeSkill_Desc() { return &m_ChargeSkill_Desc; }
	void			Set_ChargeSkill_Desc(_bool isUsing, _float fChargingTime);
	CState*			Get_State(STATE eState) { return m_States[eState]; }
	void			Set_Animation(_uint iIndex, _bool bLoop = false,_float fLerpTime = 0.1f);
	_bool			isAnimationFinish();
	

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	_bool	Move(_float fTimeDelta);
	void	TurnToCursor();
	void	Change_Stance();
	void	Add_Buff(_uint iBuffID);
	void	Play_CameraAnimation(CAMERA_ANIM eState);

private:
	PLAYER_INFO				m_DefaultInfo = {};
	PLAYER_INFO				m_PlayerInfo = {};
	
	BUFFSTAT				m_BuffStat = {};
	CHARGE_SKILL_DESC		m_ChargeSkill_Desc = {};

	CStateMachine*			m_pStateMachineCom = { nullptr };
	CState*					m_States[STATE_END] = {};
	CNavigation*			m_pNavigationCom = { nullptr };

	_float4					m_PreRootBonePosition = {};

	const _float4x4*		m_pRootBoneMatrix = { nullptr };

	list<class CBuff*>		m_Buffs;

private:
	HRESULT			Ready_Components();
	HRESULT			Ready_PartObjects();
	HRESULT			Ready_StateMachine();
	HRESULT			Ready_States();
	void			Update_Buff(_float fTimeDelta);

	void			Key_Input(_float fTimeDelta);

	void            Change_State();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
