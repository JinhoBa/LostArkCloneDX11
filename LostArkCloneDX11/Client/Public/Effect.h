#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CEffect abstract : public CPartObject
{
public:
	typedef struct Effect_Desc : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix{ nullptr };
	}EFFECT_DESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT Start(void* pArg);
	virtual HRESULT Reset();

protected:
	const _float4x4* m_pSocketMatrix = { nullptr };
	_float4x4		 m_CombindedMatrix = {};
	_uint		m_iInstanceIndex = {};
	_bool		m_isLoop = {};
	_uint		m_iNumInstance = {};

	_float		m_fLifeTime = {};

	_float2		m_vSize = {};
	_float3		m_vCenter = {};
	_float2		m_vSpeed = {};
	_float3		m_vRange = {};
	_float2		m_vLifeTime = {};
	_float3		m_vPivot = {};

	_bool		m_isActive = {};
	_float		m_fTimeAcc = {};

	_int		m_iPassIndex = {};
	_int		m_iTextureIndex = {};

private:
	HRESULT		Add_Components();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

NS_END

