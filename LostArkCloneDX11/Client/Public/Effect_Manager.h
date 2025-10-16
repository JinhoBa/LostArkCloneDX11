#pragma once
#include "Client_Defines.h"

#include "GameObject.h"
#include "GameManager.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CEffect_Manager : public CGameObject
{
private:
	CEffect_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Manager(const CEffect_Manager& Prototype);
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void Add_Effects(EFFECT eType, _uint iEffectID, const _float4x4* pWorldMatirx, void* pArg);
	HRESULT Load_Mesh_Data(const _char* pFilePath);

#ifdef _DEBUG
public:
	HRESULT Save_Effect(EFFECT eType, void* pArg, const _char* pFilePath, _uint iID);
#endif // _DEBUG


private:
	vector<EFFECT_GROUND_DESC>		m_EffectData;
	vector<EFFECT_MESH_DESC>		m_MeshEffect_Datas;
	deque<class CEffect*>			m_GroundEffects;
	deque<class CEffect*>			m_MeshEffects;
	list<class CEffect*>			m_pActiveEffects;

public:
	static CEffect_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END