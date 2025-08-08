#pragma once

#include "Client_Defines.h"

#include "Base.h"

NS_BEGIN(Client)

class CData_Manager : public CBase
{
private:
	CData_Manager();
	virtual ~CData_Manager() = default;

public:
	HRESULT Load_File(const _tchar* pFilePath);
	HRESULT Save_File(const _tchar* pFilePath);

public:
	virtual void Free() override;
};

NS_END