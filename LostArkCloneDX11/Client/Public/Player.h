#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE {IDLE, MOVE, ATTACK};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	STANCE Get_Stance() { return m_PlayerInfo.eStance; }
	PLAYER_INFO* Get_Info() { return &m_PlayerInfo; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	PLAYER_INFO				m_PlayerInfo = {};
	_float3*				m_pPickingPos = { nullptr };

	STATE					m_ePreState = {};
	STATE					m_eCurState = {};

	_bool					m_bSkillLoop = {};
	_uint					m_iSkillID = {};

	class CGameManager*		m_pGameManger = { nullptr };

	class CBody_Player*		m_pBodyPlayer = { nullptr };

	class CStateMachine*	m_pStateMachineCom = { nullptr };
	class CState_Idle*		m_pState_Idle = { nullptr };

private:
	HRESULT			Ready_PartObjects();

	void			Key_Input(_float fTimeDelta);
	void			Change_Stance();
	void            Change_State();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
