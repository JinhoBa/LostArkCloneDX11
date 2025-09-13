#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CWeapon_Kamen final : public CPartObject
{
public:
	typedef struct KamenWeapon_Desc :public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix;
	}WEAPON_KAMEN_DESC;

private:
	CWeapon_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Kamen(const CWeapon_Kamen& Prototype);
	virtual ~CWeapon_Kamen() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	const _float4x4* m_pSocketMatrix = { nullptr };

	_uint	m_iNumMesh = {};

	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = {};

private:
	HRESULT		Add_Components();

public:
	static CWeapon_Kamen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

