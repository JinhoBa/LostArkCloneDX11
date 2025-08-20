#pragma once
#include "Client_Defines.h"

#include "Level.h"

NS_BEGIN(Engine)
class CGameObject;
class CTextureMap;
NS_END

NS_BEGIN(Client)

class CLevel_MapEditor final : public CLevel
{
private:
	CLevel_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual ~CLevel_MapEditor() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CGameManager*		m_pGameManager = {};
	CTextureMap*			m_pTextureCom = { nullptr };

	_int					m_iComboIndex = {};


	vector<const _char*>	m_PrototypeTags = {};
	vector<_wstring>*		m_pImagesNames = {};

	list<CGameObject*>*		m_pBackGroundObject = { nullptr };
	list<CGameObject*>*		m_pTerrains = { nullptr };
	class CMapObject*		m_pMapObject = { nullptr };
	class CTerrain*			m_pTerrain = { nullptr };

	string m_strSaveFileName = {};

private:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Canvas(const _wstring& strLayerTag);

	HRESULT Add_MapObject();
	HRESULT Load_MapData();
	HRESULT Save_MapData();

public:
	static CLevel_MapEditor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END