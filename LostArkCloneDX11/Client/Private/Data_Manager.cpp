#include "pch.h"
#include "Data_Manager.h"

CData_Manager::CData_Manager()
{
}

HRESULT CData_Manager::Load_File(const _tchar* pFilePath)
{
    ifstream file(pFilePath);

    if (file.fail())
    {
        MSG_BOX("Failed to Load File");
        return E_FAIL;
    }

    /*while (file.good())
    {
        vector<string> row = CSV_Read();
    }*/

    return S_OK;
}

HRESULT CData_Manager::Save_File(const _tchar* pFilePath)
{
    return E_NOTIMPL;
}

void CData_Manager::Free()
{
    __super::Free();
}
