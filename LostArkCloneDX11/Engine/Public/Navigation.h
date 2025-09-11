#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CNavigation : public CComponent
{
public:
	typedef struct Navigation_Desc
	{
		_int iCurrentIndex{ -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	_bool isMove(_fvector vPosition);
	void SnapToNavMesh(class CTransform* pTransform);

public:
	virtual HRESULT Initialize_Prototype(const _char* pNavigaitonFilePath);
	virtual HRESULT Initialize(void* pArg);

	void	Update_WorldMatrix(_fmatrix WorldMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	}

#ifdef _DEBUG
public:
	virtual HRESULT	Render();
#endif // _DEBUG

private:
	_int					m_iCurrentIndex = {};

	static _float4x4		m_WorldMatrix;

	class CShader*			m_pShaderCom = { nullptr };
	vector<class CCell*>	m_Cells;

private:
	void Set_NeighborIndices();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pNavigaitonFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END