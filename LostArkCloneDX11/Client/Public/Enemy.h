#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"

#include "Character.h"

NS_BEGIN(Client)

class CEnemy abstract : public CCharacter
{
protected:
	CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemy(const CEnemy& Prototype);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void	OnHit(const ATTACK_DESC& Attack_Desc) override;

protected:
	ENEMY_INFO		m_EnemyInfo = {};
	deque<HIT_TYPE> m_HitTypes;

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END