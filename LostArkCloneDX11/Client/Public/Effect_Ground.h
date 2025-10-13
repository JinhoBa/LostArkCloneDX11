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

	virtual HRESULT Start(void* pArg) override;
	virtual HRESULT Reset() override;

public:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };
	CTexture* m_pNoiseTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

#ifdef _DEBUG
	LERP		m_eLerpType = { LERP::LINEAR };
	_bool		m_bTest = {};
	_bool		m_isRotation = {};

	_uint		m_BaseIndex = {};
	_uint		m_MaskIndex = {};
	_uint		m_NoiseIndex = {};

	_float		m_fSpeed = {};
	_float		m_fLifeTime = {};
	_float		m_fSize = {};
	_float		m_fRotationSpeed = {};

	_float3		m_vPosition = {};
	_float3		m_vPivot = {};
	_float3		m_vRotation = {};


#endif // _DEBUG


private:
	HRESULT		Add_Components();

public:
	static CEffect_Ground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

