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

	CShader*	m_pShaderCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CTexture*	m_pMaskTextureCom = { nullptr };
	CTexture*	m_pNoiseTextureCom = { nullptr };

	_uint		m_iNumMesh = {};
	_uint		m_iMeshIndex = {};

	_float3		m_vPosition = {};
	_float3		m_vRotation = {};

	_float3		m_vSize = {};
	_float2		m_vLifeTime = {};

	_float      m_fNoiseStrength = {};

	_float		fDiffuseScrollSpeedU = {};
	_float		fDiffuseScrollSpeedV = {};

	_float		fMaskScrollSpeedU = {};
	_float		fMaskScrollSpeedV = {};

	_float2		m_vDiffuseOffset = {};
	_float2		m_vMaskOffset = {};

	_wstring m_strDiffuseTexture = {};
	_wstring m_strMaskTexture = {};
	_wstring m_strNoiseTexture = {};

	vector<CModel*> m_EffectModels;

private:
	HRESULT		Add_Components();

public:
	static CTestMeshEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

