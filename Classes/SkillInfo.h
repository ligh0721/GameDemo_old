#pragma once


class COrgSkillInfo : CCObject
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
        kThunderClap1,
        kHero1,
        kThump1,
        kThrowHit1,
        kSpeedUp1,
		//kTransmit1,
        kThunderAttack1,
        kSwordStorm1,
        kJumpChop1,
        kThunderBoltBuff1,
        kHealingBuff1,
        kSpeedUpBuff1,
        kHealing1,
        kShockWave1,
        kFastStrikeBack1,
        kKnockBack1,
		kAddDamage1,
        kRunBuff1,
        kPoison1,
    };

    struct SKILL_INFO
    {
        int iIndex; // index in sm
        string sName;
        string sDesc;
        string sBtn;
    };

    typedef map<int, SKILL_INFO> MAP_SKILL_INFO;

public:
    COrgSkillInfo();
    virtual bool init();
    CREATE_FUNC_PARAM(COrgSkillInfo, ());
    static COrgSkillInfo* sharedOrgSkillInfo();
    
    virtual CSkill* skill(int iSkillIndex);

public:
    MAP_SKILL_INFO m_mapSkills;
};


