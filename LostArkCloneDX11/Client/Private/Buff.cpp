#include "pch.h"
#include "Buff.h"

CBuff::CBuff()
{
}

HRESULT CBuff::Initilize(_uint iBuffID)
{
	m_isFinished = false;
	m_fTimeAcc = 0.f;
	m_iBuffID = iBuffID;

	if (0 == m_iBuffID)
	{
		m_BuffStat.fAtk_Flat = 500.f;
		m_BuffStat.fAtk_Pct = 0.4f;
		m_BuffStat.fAtkSpeed_Pct = 0.2f;
		m_BuffStat.fMoveSpeed_Pct = 0.2f;
		m_BuffStat.Critical_Probability_Flat = 40.f;
		m_BuffStat.Critical_Damage_Pct = 0.2f;
		m_fDuration = 13.f;
	}


	return S_OK;
}

void CBuff::Update(_float fTimeDelta, BUFFSTAT* pStat)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc)
		m_isFinished = true;

	pStat->fAtk_Flat += m_BuffStat.fAtk_Flat;
	pStat->fAtk_Pct += m_BuffStat.fAtk_Pct;
	pStat->fAtkSpeed_Pct += m_BuffStat.fAtkSpeed_Pct;
	pStat->fMoveSpeed_Pct += m_BuffStat.fMoveSpeed_Pct;
	pStat->Critical_Probability_Flat += m_BuffStat.Critical_Probability_Flat;
	pStat->Critical_Damage_Pct += m_BuffStat.Critical_Damage_Pct;
}
void CBuff::Reset()
{
	ZeroMemory(&m_BuffStat, sizeof(BUFFSTAT));
}
CBuff* CBuff::Create()
{
	return new CBuff();
}

void CBuff::Free()
{
	__super::Free();
}
