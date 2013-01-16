#pragma once

#include "Unit.h"


class CHeroUpdate : public CLevelUpdate
{
public:
    virtual void updateMaxExp(CLevelExp* pLevel);
    virtual void onLevelChange(CLevelExp* pLevel, int32_t iChanged);
};

extern CHeroUpdate g_oDemoUpdate;

class CHeroUnit : public CGameUnit
{
public:
    CREATEWITH_FUNC_PARAM(Info, CHeroUnit, (const CUnitInfo& roUnitInfo), roUnitInfo);

    virtual void onLevelChange(int32_t iChanged);
    virtual void updateMaxExp();

public:
    //VEC_SKILLS m_vecSkill;
};

class CHeroInfo
{
public:
    typedef vector<int> VEC_SKILLS;
    static uint16_t CONST_FILE_DATA_SIZE;

public:
    virtual bool initWithFileStream(CGameFile* pFile);
    virtual void writeToFileStream(FILE* pFile);
    CHeroUnit* createHero();
    void updateHeroLevel(CHeroUnit* pUnit);
    void addSkill(int iOrgSkillIndex);

public:
    int m_iHeroIndex;
    VEC_SKILLS m_vecSkills; // org skill index
    uint32_t m_dwMaxLevel;
    uint32_t m_dwLevel;
    uint32_t m_dwMaxExp;
    uint32_t m_dwExp;
    
};

class CPlayerInfo
{
public:
    CPlayerInfo(void);
    virtual ~CPlayerInfo(void);

    //virtual bool initWithFile();

public:
    vector<CHeroInfo> m_vecHero;
    int m_iCurHero;
    int m_iGold;
};

