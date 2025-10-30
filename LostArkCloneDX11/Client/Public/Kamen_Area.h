#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CTexture;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CKamen_Area : public CContainerObject
{
private:
	CKamen_Area(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKamen_Area(const CKamen_Area& Prototype);
	virtual ~CKamen_Area() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	void				Update_ImGui();

private:
	_uint			m_iNumMesh = {};
	_float			m_fTimeAcc = {};
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };

#ifdef _DEBUG
	_float			m_fScale = {};
#endif

private:
	HRESULT Add_Components();

public:
	static CKamen_Area* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

