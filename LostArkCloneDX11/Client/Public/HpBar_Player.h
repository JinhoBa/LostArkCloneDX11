#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
NS_END

NS_BEGIN(Client)

class CHpBar_Player final : public CPartObject
{
public:
	typedef struct HPBarDesc : public PARTOBJECT_DESC
	{
		const PLAYER_INFO* pPlayerInfo = { nullptr };
		const _float4x4*   pSocketMatrix;
	}HPBAR_DESC;
private:
	CHpBar_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar_Player(const CHpBar_Player& Prototype);
	virtual ~CHpBar_Player() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_float				m_fValue = {};

	const PLAYER_INFO*	m_pPlayerInfo = { nullptr };
	const _float4x4*	m_pSocketMatrix = { nullptr };

	CTexture*			m_pTextureCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };

	FONT_DESC			m_PlayerNameFont = {};

private:
	HRESULT		Add_Components();
	HRESULT     Ready_Font();

public:
	static CHpBar_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

