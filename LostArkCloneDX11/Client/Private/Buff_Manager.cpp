#include "pch.h"
#include "Buff_Manager.h"

#include "Buff.h"

CBuff_Manager::CBuff_Manager()
{
}

HRESULT CBuff_Manager::Initilize()
{
	for (_uint i = 0; i < 15; ++i)
	{
		m_Buffs.push_back(CBuff::Create());
	}

	return S_OK;
}

CBuff* CBuff_Manager::Add_Buff(_uint iBuffID)
{
	CBuff* pBuff = m_Buffs.front();
	m_Buffs.pop_front();

	pBuff->Initilize(iBuffID);

	return pBuff;
}

void CBuff_Manager::Remove_Buff(CBuff* pBuff)
{
	pBuff->Reset();

	m_Buffs.push_back(pBuff);
}

CBuff_Manager* CBuff_Manager::Create()
{
	CBuff_Manager* pInstance = new CBuff_Manager();

	if (FAILED(pInstance->Initilize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CBuff_Manager");
		return nullptr;
	}
	return pInstance;
}

void CBuff_Manager::Free()
{
	__super::Free();

	for (auto& pBuff : m_Buffs)
		Safe_Release(pBuff);
}
