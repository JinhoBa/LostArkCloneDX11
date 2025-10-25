#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
NS_END

NS_BEGIN(Client)

class CWeaponEffect_Player final : public CPartObject
{
public:
	enum class STATE {IDLE, SHOOT};
public:
	typedef struct Weapon_Desc :public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix;
	}WEAPON_DESC;

private:
	CWeaponEffect_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponEffect_Player(const CWeaponEffect_Player& Prototype);
	virtual ~CWeaponEffect_Player() = default;

public:
	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;
	void Shoot(_float4* pDir);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	const STANCE*		m_pStance = { nullptr };
	const _float4x4*	m_pSocketMatrix = { nullptr };

	STATE				m_eState = { STATE::IDLE };
	_uint				m_iNumMesh = {};

	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };

	_float				m_fTimeAcc = {};
	_float				m_fSpeed = {};
	_float4				m_vShootDir = {};

	_float				m_fRimStrength = {};
	_float				m_fRimPower = {};
	_float4				m_vRimColor = {};

private:
	HRESULT		Add_Components();

public:
	static CWeaponEffect_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

