#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
NS_END

NS_BEGIN(Client)

class CEffect_BossEnter final : public CGameObject
{
private:
	CEffect_BossEnter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_BossEnter(const CEffect_BossEnter& Prototype);
	virtual ~CEffect_BossEnter() = default;

public:
	_bool isEnter() { return m_isEnter; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	EFFECT_MESH_DATA m_Effect_Data = {};

	_bool		m_isEnter = {};

	_float		m_fDissolveSpeed = {};
	_float3		m_vCurrentScale = {};
	_float2		m_vDiffuseOffset = {};
	_float2		m_vMaskOffset = {};

	_float4		m_vLightColor = {};

	_wstring	m_strDiffuseTexture = {};

	CShader*	m_pShaderCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CTexture*	m_pMaskTextureCom = { nullptr };
	CTexture*	m_pNoiseTextureCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
	CTransform* m_pPlayerTransformCom = { nullptr };


private:
	HRESULT		Add_Components();
	HRESULT     Bind_ShaderResource();

	void		Check_Enter();

public:
	static CEffect_BossEnter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

