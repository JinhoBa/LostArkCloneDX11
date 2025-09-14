#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)

class CKamen_Sword final : public CGameObject
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
	_float			m_fMaxHp = {};
	_float			m_fHp = {};

	_uint			m_iNumMesh = {};
	CTexture*		m_pDiffuseTextureCom = { nullptr };
	CTexture*		m_pEmissiveTextureCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Component();
	void Change_State();

public:
	static CKamen_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
