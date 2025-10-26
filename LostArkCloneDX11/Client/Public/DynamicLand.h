#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CShader;
class CTexture;
class CModel;
NS_END

NS_BEGIN(Client)

class CDynamicLand : public CGameObject
{
protected:
	CDynamicLand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamicLand(const CDynamicLand& Prototype);
	virtual ~CDynamicLand() = default;

public:
	void		Set_Dead() { m_isDead = true; }
	_float3*	Get_Positon() { return &m_vPosition; }
	_float3*	Get_Scale() { return &m_vScale; }
	_float3*	Get_Rotation() { return &m_vRotation; }
	_wstring&	Get_PrototypeTag();

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	void				Update_ImGui();

protected:
	_uint		m_iLevelIndex = {};
	_uint		m_iSeletPass = {};
	_uint		m_iNumMesh = {};

	_wstring	m_strPrototypeTag = {};
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CTexture*	m_pNoiseTextureCom = { nullptr };


	_wstring	m_strDiffuseTexture;
	_wstring	m_strNoiseTexture;
	_float3		m_vPosition = {};
	_float3		m_vScale = {};
	_float3		m_vRotation = {};

	_float2     m_vLifeTime = {};
	_float		m_fSpped = {};
	_float		m_fSplit = {};
	_float		m_fNoiseValue = {};
	_float4		m_vColor = {};

private:
	HRESULT Add_Components();

public:
	static CDynamicLand* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

