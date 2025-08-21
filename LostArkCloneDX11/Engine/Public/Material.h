#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CMaterials final : public CBase
{
private:
	CMaterials(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterials() = default;

public:
	HRESULT Initialize(aiMaterial* pAiMaterial, const _char* pModelFilePath);

	HRESULT Bind_SRV(class CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex);

private:
	_uint								m_iNumSRV = {};
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>	m_SRVs[ENUM_TO_INT(TEXTURE::END)] = {};

private:
	HRESULT Read_MaterialFile(const _char* pMaterialFilePath, const _char* pTextureFolderPath);
	HRESULT Add_Texture(const _char* pTextureFolderPath, string& FileType);

public:
	static CMaterials* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, aiMaterial* pAiMaterial, const _char* pFilePath);
	virtual void Free() override;
};

NS_END