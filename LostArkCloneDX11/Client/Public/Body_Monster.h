#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CBody_Monster final : public CPartObject
{
public:
	typedef struct BodyMonster_Desc : public CPartObject::PARTOBJECT_DESC
	{
		_uint		iMonterID{};
		_wstring	strModelPrototypeTag{};
	}BODYMONSTER_DESC;

private:
	CBody_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Monster(const CBody_Monster& Prototype);
	virtual ~CBody_Monster() = default;

public:
	const _bool isAnimationFinish() const {
		return m_isAnimationFinish;
	}
	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;
	void Set_Animation(_uint iAnimationIdex, _bool bLoop = false, _float fLerpTime = 0.1f);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_bool					m_isAnimationFinish = {};

	_uint					m_iMonsterID = {};
	_uint					m_iNumMesh = {};
	_int					m_iAnimIndex = {};

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	HRESULT			Add_Components(_wstring& strPrototypeTag);
	HRESULT			Bind_ShaderResources();

public:
	static CBody_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
