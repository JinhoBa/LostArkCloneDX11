#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
NS_END

NS_BEGIN(Client)

class CHpBar_Monster final : public CPartObject
{
public:
	typedef struct HPBarMonster_Desc : public PARTOBJECT_DESC
	{
		const MONSTER_INFO*	pMonsterInfo;
		const _float4x4* pSocketMatrix;
	}HPBAR_MONSTER_DESC;
private:
	CHpBar_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar_Monster(const CHpBar_Monster& Prototype);
	virtual ~CHpBar_Monster() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_float				m_fValue = {};

	const _float4x4*	m_pSocketMatrix = { nullptr };
	const MONSTER_INFO* m_pMonsterInfo = { nullptr };

	CTexture*			m_pTextureCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };

	FONT_DESC			m_NameFont = {};

private:
	HRESULT		Add_Components();
	HRESULT     Ready_Font();

public:
	static CHpBar_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

