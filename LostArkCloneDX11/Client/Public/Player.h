#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	STANCE Get_Stance() { return m_Info.eStance; }
	PLAYER_INFO* Get_Info() { return &m_Info; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	class CGameManager*		m_pGameManger = { nullptr };

	_bool					m_bMove = {};
	_bool					m_bAnimLoop = {};
	_int					m_iAnimIndex = {};
	_uint					m_iNumMesh = {};

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

	PLAYER_INFO				m_Info = {};

private:
	void			Key_Input(_float fTimeDelta);
	HRESULT			Add_Components();
	void			Change_Stance();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
