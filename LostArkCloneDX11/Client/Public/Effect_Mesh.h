#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
NS_END

NS_BEGIN(Client)

class CEffect_Mesh final : public CEffect
{
private:
	CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh(const CEffect_Mesh& Prototype);
	virtual ~CEffect_Mesh() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT Start(const _float4x4* pWorldMatrix, void* pArg)override;
	virtual HRESULT Reset()override;

private:
	EFFECT_MESH_DATA m_Effect_Data = {};

	_float		m_fDissolveSpeed = {};

	_float3		m_vCurRotation = {};
	_float3		m_vCurrentScale = {};

	_float2		m_vDiffuseOffset = {};
	_float2		m_vMaskOffset = {};

	_wstring	m_strDiffuseTexture = {};
	_wstring	m_strMaskTexture = {};
	_wstring	m_strNoiseTexture = {};

	_float4x4	m_ParentWorldMatrix = {};

	CShader*	m_pShaderCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CTexture*	m_pMaskTextureCom = { nullptr };
	CTexture*	m_pNoiseTextureCom = { nullptr };

	vector<CModel*> m_EffectModels;

private:
	HRESULT		Add_Components();
	HRESULT     Bind_ShaderResource();

public:
	static CEffect_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

