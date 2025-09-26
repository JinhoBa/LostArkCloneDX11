#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"

NS_BEGIN(Client)

class CClashUI final : public CUIPanel
{
public:
	typedef struct ClashUIDesc : public UIOBJECT_DESC
	{
		_int iKey{};
		_float2 vPosition{};
	}CLASH_UI_DESC;
private:
	CClashUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClashUI(CClashUI& Prototype);
	virtual ~CClashUI() = default;

public:
	_bool Check_Circle();

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_int			m_iKey = {};
	_uint			m_iKeyTextureIndex = {};
	_uint			m_iOutCircleTextureIndex = {};
	_float			m_fOutSize = {};
	_float			m_fKeySize = {};
	_float			m_fTimeAcc = {};

	CTexture*		m_pKeyTextureCom = { nullptr };
	CTransform*		m_pOutTransformCom = { nullptr };
	CTransform*		m_pKeyTransformCom = { nullptr };

private:
	HRESULT Ready_Components();

	virtual void Update_Position();

	HRESULT Draw_UI();

public:
	static CClashUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END