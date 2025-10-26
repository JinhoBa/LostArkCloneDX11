#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CEffect_Particle final : public CEffect
{
private:
	CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Particle(const CEffect_Particle& Prototype);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT		Start(const _float4x4* pWorldMatrix, void* pArg)override;
	virtual HRESULT		Reset()override;

private:
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };
	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CTexture*					m_pMaskTextureCom = { nullptr };
	CTexture*					m_pNoiseTextureCom = { nullptr };

	EFFECT_PARTICLE_DATA		m_Particle_Data = {};

	_wstring					m_strDiffuseTexture = {};
	_wstring					m_strMaskTexture = {};
	_wstring					m_strNoiseTexture = {};

	_bool						m_isActive = {};
	_float						m_fTimeAcc = {};

	_float4x4					m_ParentWorldMatrix = {};

	_float						m_fSizeX = {};
	_float						m_fSizeY = {};

private:
	HRESULT		Add_Components();

public:
	static CEffect_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

