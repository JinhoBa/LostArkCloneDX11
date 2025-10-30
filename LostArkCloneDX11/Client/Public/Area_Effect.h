#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
class CModel;
NS_END

NS_BEGIN(Client)

class CArea_Effect final : public CEffect
{
private:
	CArea_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArea_Effect(const CArea_Effect& Prototype);
	virtual ~CArea_Effect() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	CVIBuffer_Point_Instance*	m_pVIBufferCom = { nullptr };

	CShader*					m_pShaderCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CTexture*					m_pMaskTextureCom = { nullptr };

	PARTICLE					m_eType = {};
	_bool						m_isLoop = {};
	_uint						m_iNumInstance = {};
	_uint						m_iPassIndex = {};
	_float2						m_vSize = {};
	_float3						m_vCenter = {};
	_float2						m_vSpeed = {};
	_float3						m_vRange = {};
	_float2						m_vLifeTime = {};
	_float3						m_vPivot = {};
	_float						m_fActiveTime = {};

	_wstring					m_strDiffuseTexture = {};
	_wstring					m_strMaskTexture = {};
	_wstring					m_strNoiseTexture = {};

	_bool						m_isActive = {};
	_float						m_fTimeAcc = {};

	_float4x4					m_ParentWorldMatrix = {};

	_bool						m_isBlur = {};
	_float						m_fSizeX = {};
	_float						m_fSizeY = {};

	_float						m_fRotationSpeed = {};
	_float						m_fRotationAngle = {};
	_float3						m_vPosition = {};
	_float3						m_vRotation = {};

private:
	HRESULT		Add_Components();

public:
	static CArea_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

