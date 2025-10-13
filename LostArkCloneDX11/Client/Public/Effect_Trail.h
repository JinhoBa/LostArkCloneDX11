#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Line_Instance;
NS_END

NS_BEGIN(Client)

class CEffect_Trail final : public CEffect
{
public:
	typedef struct TrailEffect_Desc : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix{ nullptr };
	}TRAIL_EFFECT_DESC;
private:
	CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Trail(const CEffect_Trail& Prototype);
	virtual ~CEffect_Trail() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Line_Instance* m_pVIBufferCom = { nullptr };
	const _float4x4*		m_pSocketMatrix = { nullptr };
	_float4x4 m_IdentityMatrix = {};
	_float m_fWidth = {};
#ifdef _DEBUG
	_uint m_BaseIndex = {};
	_float3							m_vPosition = {};
	LERP							m_eLerpType = { LERP::LINEAR };
#endif // _DEBUG

private:
	HRESULT		Add_Components();

public:
	static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

