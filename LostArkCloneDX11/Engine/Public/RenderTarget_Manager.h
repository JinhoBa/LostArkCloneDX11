#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderTarget_Manager final : public CBase
{
private:
	CRenderTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV);
	HRESULT End_MRT();
	HRESULT Bind_RenderTarget(const _wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	ID3D11RenderTargetView*		m_BackBufferRTV = { nullptr };
	ID3D11DepthStencilView*		m_pOriginalDSV = { nullptr };

	map<const _wstring, class CRenderTarget*>			m_RenderTargets;
	map<const _wstring, list<class CRenderTarget*>>		m_MRTs;

private:
	class CRenderTarget* Find_RenderTarget(const _wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _wstring strMRTTag);

public:
	static CRenderTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END