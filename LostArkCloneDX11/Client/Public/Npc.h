#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"

#include "Character.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)

class CNpc final : public CCharacter
{
public:
	enum STATE { IDLE, TALK, FEAR, WALK, END };

private:
	CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc(const CNpc& Prototype);
	virtual ~CNpc() = default;

public:
	void Set_State(STATE eState) { m_eCurState = eState; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	STATE			m_ePreState = { STATE::END };
	STATE			m_eCurState = { STATE::END };

	_uint			m_iNumMesh = {};
	_float			m_fWalkSpeed = {};
	_float			m_fLightRange = {};

	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CNavigation*	m_pNavigationCom = { nullptr };

	CTransform*		m_pPlayerTransformCom = { nullptr };

#ifdef _DEBUG
	_int m_iAnimIndex = {};
#endif // _DEBUG


private:
	HRESULT			Ready_Components();
	void			Change_State();

public:
	static CNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
