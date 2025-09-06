#pragma once
#include "Client_Defines.h"

#include "HUD.h"

NS_BEGIN(Client)

class CUtilitySkillSlot final : public CHUD
{
public:
	typedef struct UtilitySlot_Desc : public CUIObject::UIOBJECT_DESC
	{
		_uint iSlotID{};
		_uint iTextureIndex{};
		_uint iSubTextureIndex{};
	}UTILITYSLOT_DESC;
private:
	CUtilitySkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUtilitySkillSlot(const CUtilitySkillSlot& Prototype);
	virtual ~CUtilitySkillSlot() = default;

public:
	_bool isUsed() const { return 0.f < m_fCoolTime; }

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Priority_Update(_float fTimeDelta)override;
	virtual void	Update(_float fTimeDelta)override;
	virtual void	Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	class CGameManager* m_pGameManager = { nullptr };

	_uint				m_iSlotID= {};
	_uint				m_iTextureIndex[ENUM_TO_INT(STANCE::IDEN_END)] = {};

	_float				m_fMaxCoolTime = {};
	_float				m_fCoolTime = {};

	CTexture*			m_pFrameTextureCom = { nullptr };
	CTexture*			m_pSkillTextureCom = { nullptr };

	FONT_DESC			m_Font_CoolTime = {};

protected:
	HRESULT Bind_Resource();
	HRESULT Draw();
	HRESULT Render_SlotBack();
	HRESULT Ready_Font();

private:
	HRESULT Add_Components();

public:
	static CUtilitySkillSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END
