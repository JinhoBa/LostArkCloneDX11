#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CDynamic_SkyBox : public CGameObject
{
private:
	CDynamic_SkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_SkyBox(const CDynamic_SkyBox& Prototype);
	virtual ~CDynamic_SkyBox() = default;

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

	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };


	ID3D11RasterizerState* m_pRasterState = nullptr;

private:
	HRESULT Add_Components();

public:
	static CDynamic_SkyBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

