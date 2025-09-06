#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
class ENGINE_DLL CUIBar : public CUIObject
{
public:
	typedef struct UIBar_Desc : CUIObject::UIOBJECT_DESC
	{
		_float fMax{};
		_float fStartValue{};
	}UIBAR_DESC;

protected:
	CUIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBar(const CUIBar& Prototype);
	virtual ~CUIBar() = default;

public:
	void Set_Indices(_uint iSRVIndex_Back, _uint iSRVIndex_Fill) {
		m_iSRVIndex_Back = iSRVIndex_Back;
		m_iSRVIndex_Fill = iSRVIndex_Fill;
	}

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	_uint			m_iSRVIndex_Back = {};
	_uint			m_iSRVIndex_Fill = {};

	_float			m_fMax = {};
	_float			m_fValue = {};

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	

public:
	static CUIBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END