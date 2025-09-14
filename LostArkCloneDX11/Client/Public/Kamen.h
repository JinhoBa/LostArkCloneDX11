#pragma once
#include "Client_Defines.h"
#include "Character.h"

NS_BEGIN(Engine)
class CStateMachine;
class CState;
NS_END

NS_BEGIN(Client)

class CKamen final : public CCharacter
{
public:
	enum class KAMENSTATE {INTRO, IDLE, 
		ATTACK_NORMAL, ATTACK_COMBO, ATTACK_CHARGE,
		DEAD, END };

private:
	CKamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKamen(const CKamen& Prototype);
	virtual ~CKamen() = default;

public:
	_bool	isAnimationFinish();
	CState* Get_State(KAMENSTATE eState) const {
		return m_States[ENUM_TO_INT(eState)];
	}
	void	Set_Animation(_uint iIndex, _bool bLoop = false, _float fLerpTime = 0.2f);
	void	Change_Phase(PHASE ePhase);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	PHASE				m_ePhase = { PHASE::END };
	CStateMachine*		m_pStateMachineCom = { nullptr };
	CState*				m_States[ENUM_TO_INT(KAMENSTATE::END)] = {};

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
