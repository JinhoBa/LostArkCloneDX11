#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CCollider;
NS_END

NS_BEGIN(Client)

class CKamen_Sword final : public CEnemy
{
	enum STATE {START, IDEL, HIT, DEAD};

private:
	CKamen_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKamen_Sword(const CKamen_Sword& Prototype);
	virtual ~CKamen_Sword() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	STATE			m_ePreState = {};
	STATE			m_eCurState = {};
	
	_bool			m_isAnimationFinished = {};

	_uint			m_iNumMesh = {};
	CTexture*		m_pDiffuseTextureCom = { nullptr };
	CTexture*		m_pEmissiveTextureCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CCollider*		m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Component();
	void Change_State();

public:
	static CKamen_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
