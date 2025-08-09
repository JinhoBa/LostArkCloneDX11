#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CMapObject abstract : public CGameObject
{
public:
	typedef struct MapObject_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_wstring	strPrototypeTag = {};
		_float3		vPosition{};
		_float3		vScale{};
		_float3		vRotation{};
	}MAPOBJECT_DESC;
protected:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& Prototype);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	_float3* Get_Positon() { return &m_vPosition; }
	_float3* Get_Scale() { return &m_vScale; }
	_float3* Get_Rotation() { return &m_vRotation; }

protected:
	_wstring	m_strPrototypeTag = {};
	CTexture*	m_pTextureCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

	_float3 m_vPosition = {};
	_float3 m_vScale = {};
	_float3 m_vRotation = {};

private:
	HRESULT Add_Components();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END
