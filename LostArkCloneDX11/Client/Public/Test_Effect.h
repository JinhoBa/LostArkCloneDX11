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

class CTest_Effect final : public CEffect
{
private:
	CTest_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTest_Effect(const CTest_Effect& Prototype);
	virtual ~CTest_Effect() = default;

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
	CTexture*					m_pNoiseTextureCom = { nullptr };

	_float3		m_vPosition;
	_float3		m_vRotation;

	_uint		m_iInstanceIndex = {};
	_bool		m_isLoop = {};
	_uint		m_iNumInstance = {};
	_float2		m_vSize = {};
	_float3		m_vCenter = {};
	_float2		m_vSpeed = {};
	_float3		m_vRange = {};
	_float2		m_vLifeTime = {};
	_float3		m_vPivot = {};
	_float3		m_vOffset = {};

	_bool		m_isActive = {};
	_float		m_fTimeAcc = {};

	_float4x4	m_ParentWorldMatrix = {};

	_wstring	m_strDiffuseTexture = {};
	_wstring	m_strMaskTexture = {};
	_wstring	m_strNoiseTexture = {};

private:
	HRESULT		Add_Components();

public:
	static CTest_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

