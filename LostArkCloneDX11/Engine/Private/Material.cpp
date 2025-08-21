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
HRESULT CMaterials::Read_MaterialFile(const _char* pMaterialFilePath, const _char* pTextureFolderPath)
{
	ifstream file(pMaterialFilePath);

	if (!file.is_open())
		return E_FAIL;

	_char szTextureFilePath[MAX_PATH] = {};

	string strText = {};
	string TextureName = {};
	string TextureType = {};

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

					TextureName = strText.substr(iBeginIndex + 1, iEndIndex - iBeginIndex - 1);

					strcpy_s(szTextureFilePath, pTextureFolderPath);
					strcat_s(szTextureFilePath, TextureName.c_str());
					/*strcat_s(szTextureFilePath, ".dds");*/
					strcat_s(szTextureFilePath, ".png");

					// name
					getline(file, strText);

					iBeginIndex = (_uint)strText.find_first_of('_');
					iEndIndex = (_uint)strText.rfind(' ');

					TextureType = strText.substr(iBeginIndex + 1, iEndIndex- iBeginIndex);

					// Add Texture
					if (FAILED(Add_Texture(szTextureFilePath, TextureType)))
						return E_FAIL;

					break;
				case 2:
					// value
					getline(file, strText);
					// name
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
	else
		return E_FAIL;
	
	_tchar szTextureFilePath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, pTextureFolderPath, (_int)strlen(pTextureFolderPath), szTextureFilePath, MAX_PATH);

	if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
	{
		if (FAILED(CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV)))
			return E_FAIL;
	}

	m_SRVs[ENUM_TO_INT(eTexture)].push_back(pSRV);

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