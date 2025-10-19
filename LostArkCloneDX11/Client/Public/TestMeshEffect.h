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

class CTestMeshEffect final : public CEffect
{
private:
	CTestMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTestMeshEffect(const CTestMeshEffect& Prototype);
	virtual ~CTestMeshEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_uint		m_iEffectID = {};

	_float		m_fDissolveSpeed = {};

	CShader*	m_pShaderCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CTexture*	m_pMaskTextureCom = { nullptr };
	CTexture*	m_pNoiseTextureCom = { nullptr };


	_bool		m_isLoop = {};
	_bool		m_isUseScale = {};
	_bool		m_isUseRotation = {};

	_uint		m_iNumMesh = {};
	_uint		m_iMeshIndex = {};

	_float3		m_vPosition = {};
	_float3		m_vPivot = {};

	_float3		m_vStartRotation = {};
	_float3		m_vCurRotation = {};
	_float3		m_vEndRotation = {};

	_float3		m_vStartScale = {};
	_float3		m_vCurrentScale = {};
	_float3		m_vEndScale = {};

	_float2		m_vLifeTime = {};

	_float      m_iPassIndex = {};

	_float		m_fSpeed = {};
	_float		m_fDiffuseScrollSpeedU = {};
	_float		m_fDiffuseScrollSpeedV = {};

	_float		m_fMaskScrollSpeedU = {};
	_float		m_fMaskScrollSpeedV = {};

	_float2		m_vDiffuseOffset = {};
	_float2		m_vMaskOffset = {};

	_wstring	m_strDiffuseTexture = {};
	_wstring	m_strMaskTexture = {};
	_wstring	m_strNoiseTexture = {};

	_float4x4	m_ParentWorldMatrix = {};

	class CPlayer* m_pPlayer = {};
	_float			m_fResetFrame = {};

	vector<CModel*> m_EffectModels;

private:
	HRESULT		Add_Components();
	HRESULT     Bind_ShaderResource();

public:
	static CTestMeshEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

