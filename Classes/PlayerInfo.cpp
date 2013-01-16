#include "CommInc.h"

#include "Unit.h"
#include "SkillInfo.h"
#include "PlayerInfo.h"


void CHeroUpdate::updateMaxExp( CLevelExp* pLevel )
{
    pLevel->m_dwMaxExp = 40 * pLevel->getLevel();
}

void CHeroUpdate::onLevelChange( CLevelExp* pLevel, int32_t iChanged )
{
    CGameUnit* pU = dynamic_cast<CGameUnit*>(pLevel);
    if (pU)
    {
        CAttackValue oAv;
        switch (pLevel->getLevel())
        {
        case 1:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kSlowDown1));
            break;
        case 2:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kCritical1));
            break;
        case 3:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kVamprie1));
            break;
        case 4:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kDoubleAttack1));
            break;
        case 5:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kSplash1));
            break;
        case 6:
            pU->addSkill(g_oOrgSkillInfo.skill(COrgSkillInfo::kCritical2));
            break;
        }
        pU->setMaxHp(50 + pLevel->getLevel() * 50);
        pU->setExAttackValue(CAttackValue::kSiege, CExtraCoeff(1, 1 * pLevel->getLevel()));
        pU->setExAttackValue(CAttackValue::kMagical, CExtraCoeff(1, MAX(0, (int)pLevel->getLevel() - 4)));
        pU->setExAttackSpeed(CExtraCoeff(1 + 0.2 * pLevel->getLevel(), 0));
    }
}

CHeroUpdate g_oDemoUpdate;

void CHeroUnit::onLevelChange( int32_t iChanged )
{
    
}

void CHeroUnit::updateMaxExp()
{
    m_dwMaxExp *= 2;
}

CGameUnit* CHeroInfo::createHero()
{
    M_DEF_UM(pUm);
    CGameUnit* pHero = pUm->unitByInfo(m_iIndex); // will be insteaded by patch
    //M_DEF_UPM(pUpm);
    //CGameUnit* pHero = pUpm->unitByIndex(m_iIndex);
    pHero->m_dwLvl = m_dwLevel;
    pHero->m_dwMaxLvl = m_dwMaxLevel;
    pHero->m_dwExp = m_dwExp;
    pHero->m_dwMaxExp = m_dwMaxExp;
    for (VEC_SKILLS::iterator it = m_vecSkill.begin(); it != m_vecSkill.end(); ++it)
    {
        pHero->addSkill(g_oOrgSkillInfo.skill(*it));
    }
    
    return pHero;
}

CPlayerInfo::CPlayerInfo(void)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
}
