#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CMapObject : public CGameObject
{
public:
	typedef struct MapObject_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_wstring	strPrototypeTag = {};
		_wstring    strTextureTag = {};
		_float3		vPosition{};
		_float3		vScale{};
		_float3		vRotation{};
	}MAPOBJECT_DESC;
protected:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& Prototype);
	virtual ~CMapObject() = default;

public:
	void		Set_Dead() { m_isDead = true; }
	_float3*	Get_Positon() { return &m_vPosition; }
	_float3*	Get_Scale() { return &m_vScale; }
	_float3*	Get_Rotation() { return &m_vRotation; }
	_wstring&	Get_PrototypeTag();
	_wstring&	Get_TextureTag();

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
	_uint		m_iSeletPass = {};
	_uint		m_iNumMesh = {};

	_wstring	m_strPrototypeTag = {};
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

	_float3		m_vPosition = {};
	_float3		m_vScale = {};
	_float3		m_vRotation = {};

private:
	HRESULT Add_Components();

public:
	static CMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
