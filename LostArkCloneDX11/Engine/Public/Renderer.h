#pragma once

/* ��ü�� �����ÿ� �׷������� ��ü��� ������Ʈ �Ŵ������� �߰��ϰ�, ���������� �߰��Ѵ�.(x)*/
/* �� �����Ӵ� ��ü�ȿ��� �׷������ϴ°��� �Ǵ��ϰ� �׷������Ѵٶ�� �������� ����ϴ� �۾��� �����Ѵ�. */

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� �з��Ͽ� �����Ѵ�. */
/* �����ϰ� �մ� ��ü���� ������ ������� �����Լ��� ȣ���� �ش�. �����̳ʸ� Ŭ�����ع�����. */
#include "Base.h"

NS_BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void	Render();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	ID3D11DepthStencilState*	m_pDSState_Priority = { nullptr };
	ID3D11DepthStencilState*	m_pDSState_UI = { nullptr };
	ID3D11BlendState*			m_pBlendState = { nullptr };

	_float						m_fFactor[4] = {};

	list<class CGameObject*>	m_RenderObjects[ENUM_TO_INT(RENDER::END)];

	_float4x4					m_OrthographicViewMatrix = {};
	_float4x4					m_OrthographicMatrix = {};
	
private:
	void Render_Priority();
	void Render_NonBlend();
	void Render_Blend();
	void Render_UI();

	void Sort_AlphaObject();
	void Sort_UI();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

NS_END