#pragma once

#include "Client_Defines.h"
#include "Monster.h"

NS_BEGIN(Client)
class CMonster_Named final : public CMonster
{

private:
	CMonster_Named(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Named(const CMonster_Named& Prototype);
	virtual ~CMonster_Named() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_States();

public:
	static CMonster_Named* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END