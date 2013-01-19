#include "CommInc.h"

#include "Unit.h"
#include "SkillInfo.h"
#include "PlayerInfo.h"
#include "GameFile.h"


void CHeroUpdate::updateMaxExp( CLevelExp* pLevel )
{
    pLevel->m_dwMaxExp = 100 * pLevel->getLevel();
}

void CHeroUpdate::onLevelChange( CLevelExp* pLevel, int32_t iChanged )
{
    CGameUnit* pU = dynamic_cast<CGameUnit*>(pLevel);
    if (!pU)
    {
        return;
    }

    pU->setMaxHp(50 + pLevel->getLevel() * 50);

    CAttackValue oAv;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        float f = pU->getBaseAttackValue((CAttackValue::ATTACK_TYPE)i);
        if (f > FLT_EPSILON)
        {
            oAv.setAttack((CAttackValue::ATTACK_TYPE)i, f + 4);
        }
    }
    pU->setBaseAttackValue(oAv);

    return;

    //CAttackValue oAv;
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

CHeroUpdate g_oDemoUpdate;

void CHeroUnit::onLevelChange( int32_t iChanged )
{
    
}

void CHeroUnit::updateMaxExp()
{
    m_dwMaxExp *= 2;
}

CHeroUnit* CHeroInfo::createHero()
{
    M_DEF_UM(pUm);
    CHeroUnit* pHero = pUm->heroByInfo(m_iHeroIndex); // will be insteaded by patch
    //M_DEF_UPM(pUpm);
    //CGameUnit* pHero = pUpm->unitByIndex(m_iIndex);
    pHero->m_dwLvl = m_dwLevel;
    pHero->m_dwMaxLvl = m_dwMaxLevel;
    pHero->m_dwExp = m_dwExp;
    pHero->m_dwMaxExp = m_dwMaxExp;
    for (VEC_SKILLS::iterator it = m_vecSkills.begin(); it != m_vecSkills.end(); ++it)
    {
        pHero->addSkill(g_oOrgSkillInfo.skill(*it));
    }
    
    return pHero;
}

uint16_t CHeroInfo::CONST_FILE_DATA_SIZE
    = sizeof(m_iHeroIndex)
    + sizeof(m_dwExp)
    + sizeof(m_dwMaxExp)
    + sizeof(m_dwLevel)
    + sizeof(m_dwMaxLevel);

bool CHeroInfo::initWithFileStream( CGameFile* pFile )
{
    uint16_t wDataSize = 0;
    if (pFile->read(&wDataSize) != 1)
    {
        return false;
    }
    size_t uPos = pFile->tell();
    bool bRet = false;
    do
    {
        m_vecSkills.clear();
        uint16_t wSkillCount;
        if (wDataSize != CONST_FILE_DATA_SIZE
            || pFile->read(&m_iHeroIndex) != 1
            || pFile->read(&m_dwMaxLevel) != 1
            || pFile->read(&m_dwLevel) != 1
            || pFile->read(&m_dwMaxExp) != 1
            || pFile->read(&m_dwExp) != 1
            || pFile->read(&wSkillCount) != 1)
        {
            break;
        }
        bool bBreak = false;
        for (uint16_t i = 0; i < wSkillCount; ++i)
        {
            int iSkill;
            if (pFile->read(&iSkill) != 1)
            {
                bBreak = true;
                break;
            }
            m_vecSkills.push_back(iSkill);
        }
        if (bBreak)
        {
            break;
        }

        bRet = true;

    } while (false);

    if (!bRet)
    {
        pFile->seek(uPos, CGameFile::kBegin);
    }
    return bRet;
}

void CHeroInfo::writeToFileStream( FILE* pFile )
{
    fwrite(&CONST_FILE_DATA_SIZE, sizeof(CONST_FILE_DATA_SIZE), 1, pFile);
    fwrite(&m_iHeroIndex, sizeof(m_iHeroIndex), 1, pFile);
    fwrite(&m_dwMaxLevel, sizeof(m_dwMaxLevel), 1, pFile);
    fwrite(&m_dwLevel, sizeof(m_dwLevel), 1, pFile);
    fwrite(&m_dwMaxExp, sizeof(m_dwMaxExp), 1, pFile);
    fwrite(&m_dwExp, sizeof(m_dwExp), 1, pFile);
    uint16_t wSkillCount = m_vecSkills.size();
    fwrite(&wSkillCount, sizeof(wSkillCount), 1, pFile);
    for (uint16_t i = 0; i < wSkillCount; ++i)
    {
        fwrite(&m_vecSkills[i], sizeof(m_vecSkills[i]), 1, pFile);
    }
}

void CHeroInfo::updateHeroLevel( CHeroUnit* pUnit )
{
    m_dwMaxLevel = pUnit->getMaxLevel();
    m_dwLevel = pUnit->getLevel();
    m_dwMaxExp = pUnit->getMaxExp();
    m_dwExp = pUnit->getExp();
}

void CHeroInfo::addSkill( int iOrgSkillIndex )
{
    m_vecSkills.push_back(iOrgSkillIndex);
}

CPlayerInfo::CPlayerInfo(void)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
}

