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

class CTest_Ground_Effect final : public CEffect
{
public:
	typedef struct EffectGroundComponets
	{
		CTexture* m_pTextureCom = { nullptr };
		CTexture* m_pMaskTextureCom = { nullptr };
		CTexture* m_pNoiseTextureCom = { nullptr };
	}EFFECT_GROUND_COM;

private:
	CTest_Ground_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTest_Ground_Effect(const CTest_Ground_Effect& Prototype);
	virtual ~CTest_Ground_Effect() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	_bool						m_isLoop = {};
	_bool						m_isRotation = {};

	LERP						m_eLerpType = { LERP::LINEAR };

	_int						m_iPassIndex = {};
	_int						m_iEffectID = {};

	_float						m_fSpeed = {};
	_float						m_fRotationSpeed = {};

	_float2						m_vSize = {};
	_float2						m_vLifeTime = {};

	_float3						m_vPosition = {};
	_float3						m_vPivot = {};
	_float3						m_vStratRotation = {};
	_float3						m_vEndRotation = {};

	_wstring					m_strDiffuseTexture = {};
	_wstring					m_strMaskTexture = {};
	_wstring					m_strNoiseTexture = {};

	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CTexture*					m_pMaskTextureCom = { nullptr };
	CTexture*					m_pNoiseTextureCom = { nullptr };

	CVIBuffer_Point_Instance*	m_pVIBufferCom = { nullptr };

private:
	HRESULT		Add_Components();

public:
	static CTest_Ground_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

