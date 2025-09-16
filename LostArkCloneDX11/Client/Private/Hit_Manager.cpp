#include "pch.h"
#include "Hit_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "Monster.h"
#include "Kamen.h"
#include "Player.h"

CHit_Manager::CHit_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() },
    m_pGameManager{ CGameManager::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGameManager);
}

void CHit_Manager::Add_HitDesc(void* pDesc)
{
    HIT_DESC* pHitDesc = static_cast<HIT_DESC*>(pDesc);

    HIT_DESC Desc = {};
    memcpy(&Desc, pHitDesc, sizeof(HIT_DESC));

    m_HitDesc_List.push_back(Desc);
}

void CHit_Manager::Update_HitData()
{
    for (auto& Desc : m_HitDesc_List)
    {
        if (CHARACTER::PLAYER == Desc.eType)
        {
            SKILL_INFO* pSkillInfo = m_pGameManager->Get_SkillInfo_Prt(Desc.iSkillID);

            _float fDamage = pSkillInfo->Damages[Desc.iHitIndex] + Desc.fBaseDamage;

            CMonster* pMonster = dynamic_cast<CMonster*>(Desc.pHitObject);

            if (nullptr != pMonster)
            {
                pMonster->OnHit(fDamage, pSkillInfo->eAttackType, pSkillInfo->eHitType);
                continue;
            }

           /* CKamen* pKamen = dynamic_cast<CKamen*>(Desc.pHitObject);
            if (nullptr != pKamen)
            {
                pKamen->OnHit(fDamage, pSkillInfo->eAttackType, pSkillInfo->eHitType);
            }*/

        }
    }

    m_HitDesc_List.clear();
}

CHit_Manager* CHit_Manager::Create()
{
    return new CHit_Manager();
}

void CHit_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGameManager);
}
