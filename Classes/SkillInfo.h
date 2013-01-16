#pragma once


class COrgSkillInfo
{
public:
    enum SKILL_INDEX
    {
        kSlowDown1 = 0,
        kHpChange1,
        kHpChange2,
        kHpChangeAura1,
        kSpeedAura1,
        kImmo1,
        kCritical1,
        kCritical2,
        kVamprie1,
        kVamprie2,
        kDoubleAttack1,
        kSplash1,
        kThunderClap1
    };

    struct SKILL_INFO
    {
        int iIndex; // index in sm
        string sName;
        string sDesc;
    };

    typedef map<int, SKILL_INFO> MAP_SKILL_INFO;

public:
    COrgSkillInfo();

    virtual bool init();
    //CC_DEPRECATED_ATTRIBUTE virtual CSkill* copySkill(int iSkillIndex);
    virtual CSkill* skill(int iSkillIndex);

public:
    MAP_SKILL_INFO m_mapSkills;
};

extern COrgSkillInfo g_oOrgSkillInfo;

