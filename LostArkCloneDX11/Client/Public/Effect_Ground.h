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

class CEffect_Ground final : public CEffect
{
private:
	CEffect_Ground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Ground(const CEffect_Ground& Prototype);
	virtual ~CEffect_Ground() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT Start(const _float4x4* pWorldMatrix, void* pArg) override;
	virtual HRESULT Reset() override;

	void Add_EffectComponets(class CTexture* pTextureCom, class CTexture* pMaskTextureCom, class CTexture* pNoiseTextureCom);

private:
	EFFECT_GROUND_DATA			m_Effect_Data = {};

	_wstring					m_strDiffuseTexture;
	_wstring					m_strMaskTexture;
	_wstring					m_strNoiseTexture;

	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CTexture*					m_pMaskTextureCom = { nullptr };
	CTexture*					m_pNoiseTextureCom = { nullptr };

	CVIBuffer_Point_Instance*	m_pVIBufferCom = { nullptr };

	_float4x4					m_ParentWorldMatrix = {};
private:
	HRESULT		Add_Components(void* pArg);


public:
	static CEffect_Ground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

