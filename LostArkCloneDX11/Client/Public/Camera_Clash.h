#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Clash final : public CCamera
{
	enum class CLASH_CAMERA {START, LOOP};
private:
	CCamera_Clash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Clash(const CCamera_Clash& Prototype);
	virtual ~CCamera_Clash() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual void		Reset() override;

	void				Set_Position(_fvector vPosistion);

private:
	CLASH_CAMERA	m_eState = {};

	_float			m_fLerp = {};
	_float			m_fClashFovy = {};

	_float3			m_vStartPosition = {};
	_float3			m_vEndPosition = {};
	_float3			m_vClashingPosition = {};

#pragma region TEST
	_float3		m_vPlayerPos = {};
	_float3		m_vStartOffset = {};
	_float3		m_vEndOffset = {};
	_float3		m_vClashingOffset = {};

#pragma endregion



private:
	void		Update_Camera_Position(_float fTimeDelta);

public:
	static CCamera_Clash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
