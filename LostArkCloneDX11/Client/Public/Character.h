#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CStateMachine;
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
	virtual void		OnHit(const ATTACK_DESC& Attack_Desc);

protected:
	class CGameManager*		m_pGameManager = { nullptr };

	_bool					m_isColl = {};
	_float4					m_PreRootBonePosition = {};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END
