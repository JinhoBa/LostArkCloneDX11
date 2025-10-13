#pragma once

#include "Texture.h"
#include "TextureMap.h"
#include "Transform.h"
//#include "VIBuffer_Cube.h"
#include "Navigation.h"
#include "VIBuffer_Cell.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Line_Instance.h"
#include "Shader.h"
#include "UIAnimation.h"
#include "Model.h"
#include "Mesh.h"
#include "StateMachine.h"
#include "Collider.h"

NS_BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	_uint										m_iNumLevels = { };
	map<const _wstring, class CBase*>*			m_pPrototypes = { nullptr };

private:
	class CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

NS_END