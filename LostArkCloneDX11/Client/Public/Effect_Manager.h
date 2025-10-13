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
	void Add_Effects(EFFECT eType, _uint iEffectID, void* pArg);
	HRESULT Load_Data(const _char* pFilePath);


private:
	vector<EFFECT_GROUND_DESC>		m_EffectData;
	deque<class CEffect*>			m_GroundEffects;
	list<class CEffect*>			m_pActiveEffects;

public:
	static CEffect_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
