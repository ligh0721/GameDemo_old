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
};

class CHeroInfo
{
public:
    typedef vector<int> VEC_SKILLS;

public:
    CGameUnit* createHero();

public:
    int m_iIndex;
    VEC_SKILLS m_vecSkill;
    uint32_t m_dwLevel;
    uint32_t m_dwMaxLevel;
    uint32_t m_dwExp;
    uint32_t m_dwMaxExp;
};

class CPlayerInfo
{
public:
    CPlayerInfo(void);
    virtual ~CPlayerInfo(void);

public:
    vector<CHeroInfo> m_vecHero;
    int m_iCurHero;
    int m_iGold;
};

