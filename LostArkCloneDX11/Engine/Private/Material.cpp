#include "Material.h"

#include "GameInstance.h"

CMaterials::CMaterials(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{pDevice}, m_pContext{pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMaterials::Initialize(aiMaterial* pAiMaterial, const _char* pModelFilePath)
{
	_char szDrive[MAX_PATH] = {};
	_char szDir[MAX_PATH] = {};
	_char szFileName[MAX_PATH] = {};

	_char szMaterialFilePath[MAX_PATH] = {};
	_char szTextureFilePath[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, nullptr, 0);

	strcpy_s(szMaterialFilePath, szDrive);
	strcat_s(szMaterialFilePath, szDir);
	strcat_s(szMaterialFilePath, "MaterialInstance/");
	strcat_s(szMaterialFilePath, pAiMaterial->GetName().data);

	string strTmp = string(szMaterialFilePath);

	auto size = strTmp.rfind('.');

	if (3 < size)
	{
		strTmp = strTmp.substr(0, size);
	}
	strcpy_s(szMaterialFilePath, strTmp.c_str());

	strcat_s(szMaterialFilePath, ".props.txt");

	strcpy_s(szTextureFilePath, szDrive);
	strcat_s(szTextureFilePath, szDir);
	strcat_s(szTextureFilePath, "Texture2D/");

	if (FAILED(Read_MaterialFile(szMaterialFilePath, szTextureFilePath)))
		return E_FAIL;

	return S_OK;
}
HRESULT CMaterials::Bind_SRV(CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex)
{
	return pShader->Bind_Resource(pConstantName, m_SRVs[ENUM_TO_INT(eTextureType)][iTextureIndex]);
}
HRESULT CMaterials::Bind_Value(CShader* pShader, const _char* pConstantName, TEXTURE eTextureType, _uint iTextureIndex)
{
	if(m_vectorValues[ENUM_TO_INT(eTextureType)].empty())
	{
		_float4 DefaultValue = { 1.f, 1.f, 1.f, 1.f };
		return pShader->Bind_Value(pConstantName, &DefaultValue);
	}

	return pShader->Bind_Value(pConstantName, &m_vectorValues[ENUM_TO_INT(eTextureType)][iTextureIndex]);
}
HRESULT CMaterials::Read_MaterialFile(const _char* pMaterialFilePath, const _char* pTextureFolderPath)
{
	ifstream file(pMaterialFilePath);

	if (!file.is_open())
	{
		MSG_BOX("Failed to Open Materail File");
		return E_FAIL;
	}

	_char szTextureFilePath[MAX_PATH] = {};

	string strText = {};

	string Value = {};
	string Name = {};

	_uint iDataIndex = {};
	_uint iNumParameter = {};

	_uint iBeginIndex = {};
	_uint iEndIndex = {};

	_bool bParent = { false };

	getline(file, strText);

	while (file)
	{
		if (2 < iDataIndex)
			break;

		if (false == bParent)
		{
			getline(file, strText);
			auto index = strText.find_first_of("[");

			iNumParameter = strText[index + 1] - '0';

			getline(file, strText);
			bParent = true;
		}
		else
		{
			if (0 == iNumParameter)
			{
				++iDataIndex;
				auto index = strText.find_first_of("[");

				iNumParameter = strText[index + 1] - '0';
				getline(file, strText);
			}
			for (_uint i = 0; i < iNumParameter; ++i)
			{
				for (_uint j = 0; j < 3; ++j)
					getline(file, strText);

				getline(file, strText);

				switch (iDataIndex)
				{
				case 0:
					// value
					getline(file, strText);
					// name
					break;
				case 1:
					// value
					iBeginIndex = (_uint)strText.find_first_of('.');
					iEndIndex = (_uint)strText.rfind('\'');

					if (128 < iBeginIndex)
						Value = "none";
					else
						Value = strText.substr(iBeginIndex + 1, iEndIndex - iBeginIndex - 1);

					strcpy_s(szTextureFilePath, pTextureFolderPath);
					strcat_s(szTextureFilePath, Value.c_str());

					// name
					getline(file, strText);

					iBeginIndex = (_uint)strText.find_first_of('_');
					iEndIndex = (_uint)strText.rfind(' ');
					
					Name = strText.substr(iBeginIndex + 1, iEndIndex- iBeginIndex);

					// Add Texture
					if (FAILED(Add_Texture(szTextureFilePath, Name)))
						return E_FAIL;

					break;
				case 2:
					// value
						// value
					iBeginIndex = (_uint)strText.find_first_of('{');
					iEndIndex = (_uint)strText.rfind('}');
					
					Value = strText.substr(iBeginIndex + 1, iEndIndex - iBeginIndex - 1);

					// name
					getline(file, strText);

					iBeginIndex = (_uint)strText.find_first_of('=');

					Name = strText.substr(iBeginIndex + 2);

					if (FAILED(Add_VectorValue(Value, Name)))
						return E_FAIL;

					break;
				default:
					break;
				}
				getline(file, strText);
			}
			++iDataIndex;
			bParent = false;
			getline(file, strText);
		}
	}

	file.close();

	return S_OK;
}

HRESULT CMaterials::Add_Texture(const _char* pTextureFolderPath, string& FileType)
{
	ID3D11ShaderResourceView* pSRV = {};

	TEXTURE eTexture = {};

	if (!strcmp(FileType.c_str(), "diffuse"))
		eTexture = TEXTURE::DIFFUSE;
	else if (!strcmp(FileType.c_str(), "normal"))
		eTexture = TEXTURE::NORMAL;
	else if (!strcmp(FileType.c_str(), "specular"))
		eTexture = TEXTURE::SPECULAR;
	else if (!strcmp(FileType.c_str(), "emissive"))
		eTexture = TEXTURE::EMISSIVE;
	else if (!strcmp(FileType.c_str(), "reflection"))
		eTexture = TEXTURE::REFLECTION;
	else if (!strcmp(FileType.c_str(), "mask_variation"))
		eTexture = TEXTURE::MASK;
	else if (!strcmp(FileType.c_str(), "color_fx_skin"))
		eTexture = TEXTURE::COLOR_FX;
	else if (!strcmp(FileType.c_str(), "cutting_mask"))
		eTexture = TEXTURE::MASK;
	else if (!strcmp(FileType.c_str(), "none"))
	{
		eTexture = TEXTURE::NONE;
		return S_OK;
	}
	else
	{
		if (!strcmp(FileType.c_str(), "eye_iristexture_ui"))
			eTexture = TEXTURE::MASK;
		else if (!strcmp(FileType.c_str(), "diffuse_base"))
			eTexture = TEXTURE::DIFFUSE;
		else if (!strcmp(FileType.c_str(), "tdspecular"))
			eTexture = TEXTURE::SPECULAR;
		else
		{
			MSG_BOX("Failed to Path Material Texture Type");
			return E_FAIL;
		}
	}
	_char TexturePath[MAX_PATH] = {};

	strcpy_s(TexturePath, pTextureFolderPath);
	strcat_s(TexturePath, ".dds");
	
	_tchar szTextureFilePath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, TexturePath, (_int)strlen(TexturePath), szTextureFilePath, MAX_PATH);

	if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
	{
		_char TexturePath_Png[MAX_PATH] = {};

		strcpy_s(TexturePath_Png, pTextureFolderPath);
		strcat_s(TexturePath_Png, ".png");

		_tchar szTextureFilePath_Png[MAX_PATH] = {};

		MultiByteToWideChar(CP_ACP, 0, TexturePath_Png, (_int)strlen(TexturePath_Png), szTextureFilePath_Png, MAX_PATH);

		if(FAILED(CreateWICTextureFromFile(m_pDevice, szTextureFilePath_Png, nullptr, &pSRV)))
		{
			MSG_BOX("Failed to Load TextureFile");
			return E_FAIL;
		}
	}
	

	m_SRVs[ENUM_TO_INT(eTexture)].push_back(pSRV);

	return S_OK;
}

HRESULT CMaterials::Add_VectorValue(string& strValue, string& strName)
{
	TEXTURE eTexture = {};

	if (!strcmp(strName.c_str(), "diffusecolor"))
		eTexture = TEXTURE::DIFFUSE;
	else
		return S_OK;

	_float4 vValue = {};

	auto iRed = strValue.find("R=");
	auto iGreen = strValue.find("G=");
	auto iBlue = strValue.find("B=");
	auto iAlpha = strValue.find("A=");

	vValue.x = stof(strValue.substr(iRed + 2, iGreen - 2));
	vValue.y = stof(strValue.substr(iGreen + 2, iBlue - 2));
	vValue.z = stof(strValue.substr(iBlue + 2, iAlpha - 2));
	vValue.w = stof(strValue.substr(iAlpha + 2));

	m_vectorValues[ENUM_TO_INT(eTexture)].push_back(vValue);


	return S_OK;
}

CMaterials* CMaterials::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, aiMaterial* pAiMaterial, const _char* pFilePath)
{
	CMaterials* pInstance = new CMaterials(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pAiMaterial, pFilePath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CMaterial");
		return nullptr;
	}

	return pInstance;
}

void CMaterials::Free()
{
	__super::Free();

	for (auto& pSRVs : m_SRVs)
	{
		for (auto& pSRV : pSRVs)
			Safe_Release(pSRV);
		pSRVs.clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}