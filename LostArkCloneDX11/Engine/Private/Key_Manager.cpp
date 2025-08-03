#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
    ZeroMemory(m_CurKeyInput, sizeof(MAX_KEY));
}

void CKey_Manager::Update_KeyInput()
{
    ZeroMemory(m_CurKeyInput, sizeof(MAX_KEY));

    for (_uint i = 0; i < MAX_KEY; ++i)
    {
        if (GetAsyncKeyState(i) & 0x8000)
            m_CurKeyInput[i] = true;
    }
}

void CKey_Manager::End_KeyInput()
{
    memcpy(m_PreKeyInput, m_CurKeyInput, sizeof(MAX_KEY));
}

_bool CKey_Manager::KeyDown(_uint iKey)
{
    return !m_PreKeyInput[iKey] && m_CurKeyInput[iKey];
}

_bool CKey_Manager::KeyPressing(_uint iKey)
{
    return m_PreKeyInput[iKey] && m_CurKeyInput[iKey];
}

_bool CKey_Manager::KeyUp(_uint iKey)
{
    return m_PreKeyInput[iKey] && !m_CurKeyInput[iKey];
}

CKey_Manager* CKey_Manager::Create()
{
    return new CKey_Manager();
}

void CKey_Manager::Free()
{
    __super::Free();
}
