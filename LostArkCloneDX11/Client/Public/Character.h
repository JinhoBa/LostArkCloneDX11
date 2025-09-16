#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CStateMachine;
class CState;
class CNavigation;
NS_END

NS_BEGIN(Client)

class CCharacter abstract : public CContainerObject
{
protected:
	CCharacter(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual void		Check_Navigation(class CNavigation* pNavigation, const _float4x4* pRootBoneMatrix);
	virtual void		OnHit(_float fDamage, ATTACK_TYPE eAttackType, HIT_TYPE eHitType)PURE;

protected:
	class CGameManager*		m_pGameManager = { nullptr };
	_float4					m_PreRootBonePosition = {};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END
