#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
NS_END

NS_BEGIN(Client)

class CBody_Kamen final : public CPartObject
{
//public:
//	typedef struct BodyKamen_Desc : public CPartObject::PARTOBJECT_DESC
//	{
//
//	}BODYKAMEN_DESC;

private:
	CBody_Kamen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Kamen(const CBody_Kamen& Prototype);
	virtual ~CBody_Kamen() = default;

public:
	const _bool isAnimationFinish() const {
		return m_isAnimationFinish;
	}
	_float				Get_TrackPoisiton();
	const _float4x4*	Get_BoneMatrixPtr(const _char* pBoneName) const;
	void				Set_Animation(_uint iAnimationIdex, _bool bLoop = false, _float fLerpTime = 0.2f);

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	_bool					m_isAnimationFinish = {};

	_uint					m_iNumMesh = {};
	_int					m_iAnimIndex = {};

#pragma region TESTCODE
	_float m_Pos[3] = {};
#pragma endregion


	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CTexture*				m_pEmssiveTextureCom = { nullptr };

private:
	HRESULT			Add_Components();
	HRESULT			Bind_ShaderResources();

public:
	static CBody_Kamen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
