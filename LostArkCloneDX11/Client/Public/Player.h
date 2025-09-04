#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CStateMachine;
class CState;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE {IDLE, MOVE, NORMAL_SKILL, DASH, CHANGE_STANCE, HIT, STATE_END};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	const STANCE	Get_Stance() { return m_PlayerInfo.eStance; }
	PLAYER_INFO*	Get_Info() { return &m_PlayerInfo; }
	CState*			Get_State(STATE eState) { return m_States[eState]; }
	void			Set_Animation(_uint iIndex, _bool bLoop = false);
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
	void	Change_Stance();

private:
	class CGameManager*		m_pGameManager = { nullptr };

	PLAYER_INFO				m_PlayerInfo = {};
	_float3*				m_pPickingPos = { nullptr };

	class CStateMachine*	m_pStateMachineCom = { nullptr };
	class CState*			m_States[STATE_END] = {};

private:
	HRESULT			Ready_PartObjects();
	HRESULT			Ready_StateMachine();
	HRESULT			Ready_States();

	void			Key_Input(_float fTimeDelta);

	void            Change_State();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
