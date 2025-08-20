#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	typedef struct Terrain_Desc : CGameObject::GAMEOBJECT_DESC
	{
		_float2 vSize = {};
		_float3 vPosition = {};
		_float3 vRotation = {};
	}TERRAIN_DESC;
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	_float4* Get_Positon() { return &m_vPosition; }
	_float3* Get_Rotation() { return &m_vRotation; }
	_float2 Get_Size() { return _float2((_float)m_iSizeX, (_float)m_iSizeZ); }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	void				Update_ImGui();

private:
	_bool					m_bVisible = {};
	CVIBuffer_Terrain*		m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

	ID3D11RasterizerState*	m_pRasterState = nullptr;

	_int					m_iSizeX = {  };
	_int					m_iSizeZ = {  };

	_float3					m_pPickingPos = {};
	_float3					m_vRotation = {};
	_float4					m_vPosition = {};

private:
	HRESULT Add_Components();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END