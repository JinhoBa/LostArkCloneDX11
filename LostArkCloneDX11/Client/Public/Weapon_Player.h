#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CWeapon_Player final : public CPartObject
{
public:
	typedef struct Weapon_Desc :public CPartObject::PARTOBJECT_DESC
	{
		const STANCE*		pStance;
		const _float4x4*	pSocketMatrix;
	}WEAPON_DESC;

private:
	CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player(const CWeapon_Player& Prototype);
	virtual ~CWeapon_Player() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	const STANCE*			m_pStance = { nullptr };
	const _float4x4*		m_pSocketMatrix = { nullptr };

	_uint					m_iNumMesh[ENUM_TO_INT(STANCE::IDEN_END)] = {};

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom[ENUM_TO_INT(STANCE::IDEN_END)] = {};
	
private:
	HRESULT		Add_Components();

public:
	static CWeapon_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

