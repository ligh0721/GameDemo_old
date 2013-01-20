#include "CommInc.h"

#include "GameDisplay.h"
#include "GameLogic.h"
#include "TankSprite.h"
#include "Skill.h"
#include "Unit.h"
#include "Package.h"
#include "Action.h"
#include "MissionInfo.h"
#include "GameCtrl.h"
#include "GameFile.h"
#include "UnitInfo.h"
#include "SkillInfo.h"


// CLevelExp
CLevelExp::CLevelExp()
    : m_dwLvl(0)
    , m_dwMaxLvl(0)
    , m_dwExp(0)
    , m_dwMaxExp(0)
    , m_pUpdate(NULL)
{
}

CLevelExp::~CLevelExp()
{
}

void CLevelExp::updateMaxExp()
{
    if (m_pUpdate)
    {
        m_pUpdate->updateMaxExp(this);
    }
    else
    {
        m_dwMaxExp = m_dwExp + 1;
    }
}

void CLevelExp::onLevelChange(int32_t iChanged)
{
    if (m_pUpdate)
    {
        m_pUpdate->onLevelChange(this, iChanged);
    }
}

void CLevelExp::addLevel(uint32_t dwLvl)
{
    setLevel(m_dwLvl + dwLvl);
}

void CLevelExp::addExp(uint32_t dwExp)
{
    float fOldMaxExp;
    m_dwExp += dwExp;
    while (m_dwExp >= m_dwMaxExp)
    {
        ++m_dwLvl;
        fOldMaxExp = m_dwMaxExp;
        updateMaxExp();
        m_dwExp -= fOldMaxExp;
        onLevelChange(1);
    }
}

void CLevelExp::setLevel(uint32_t dwLvl)
{
    uint32_t dwOldLvl = m_dwLvl;

    if (m_dwLvl > m_dwMaxLvl)
    {
        m_dwLvl = m_dwMaxLvl;
    }
    else if (m_dwLvl == m_dwMaxLvl)
    {
        m_dwLvl = m_dwMaxLvl;
    }
    else if (m_dwLvl < 1)
    {
        m_dwLvl = 1;
    }

    int32_t iChanged = m_dwLvl - dwOldLvl;

    if (iChanged)
    {
        if (m_dwLvl == m_dwMaxLvl)
        {
            m_dwExp = 0;
        }
        onLevelChange(iChanged);
        updateMaxExp();
    }
}

void CLevelExp::setMaxLevel(uint32_t dwMaxLvl)
{
    dwMaxLvl <= 0 && (dwMaxLvl = 1);
    m_dwMaxLvl = dwMaxLvl;
    setLevel(m_dwLvl);
}

uint32_t CLevelExp::getLevel() const
{
    return m_dwLvl;
}

uint32_t CLevelExp::getMaxLevel() const
{
    return m_dwMaxLvl;
}

uint32_t CLevelExp::getExp() const
{
    return m_dwExp;
}

uint32_t CLevelExp::getMaxExp() const
{
    return m_dwMaxExp;
}

void CLevelExp::setLevelUpdate( CLevelUpdate* pUpdate )
{
    m_pUpdate = pUpdate;
}








// CLife
CLife::CLife()
    : m_fHp(1)
    , m_fMaxHp(1)
{

}

bool CLife::init()
{
    return true;
}

bool CLife::isDead() const
{
    return m_fHp <= 0;
}

void CLife::onRevive()
{

}

void CLife::onDie()
{

}

bool CLife::revive(float fHp)
{
    if (isDead())
    {
        m_fHp = fHp > m_fMaxHp ? m_fMaxHp : (fHp > 0 ? fHp : 1);
        onRevive();
        return true;
    }

    return false;
}

bool CLife::setHp(float fHp)
{
    if (isDead())
    {
        return false;
    }

    float fOldHp = m_fHp;
    m_fHp = fHp < m_fMaxHp ? fHp : m_fMaxHp;
    if (m_fHp != fOldHp)
    {
        onHpChange(m_fHp - fOldHp);
    }

    if (m_fHp <= 0)
    {
        onDie();
    }

    return true;
}

void CLife::setMaxHp(float fMaxHp)
{
    float fOldMaxHp = m_fMaxHp;
    m_fMaxHp = fMaxHp > 0 ? fMaxHp : 1;
    float fNewHp = m_fHp * m_fMaxHp / fOldMaxHp;
    fNewHp < 1 && (fNewHp = 1);
    setHp(fNewHp);
}

CLife::~CLife()
{

}

void CLife::onHpChange(float fChanged)
{
}

const char* CAttackValue::CONST_ARR_NAME[CONST_MAX_ATTACK_TYPE][CONST_MAX_NAME_INDEX] = {
    { "kPhysical", "物理" },
    { "kMagical", "魔法" },
    { "kSiege", "攻城" },
    { "kHoly", "神圣" }
};

CAttackValue::CAttackValue()
{
    setZero();
}

CAttackValue::CAttackValue( int iCount, ATTACK_TYPE eType1, float fValue1, ... )
{
    setZero();
    setAttack(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (ATTACK_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setAttack(eType1, fValue1);
    }
    va_end(argv);
}

CAttackValue::~CAttackValue()
{

}

float CAttackValue::getAttack(ATTACK_TYPE eAttackType) const
{
    if (eAttackType > CONST_MAX_ATTACK_TYPE)
    {
        return 0;
    }
    return m_afAttack[eAttackType];
}

float CAttackValue::getAttack( int iAttackType ) const
{
    if (iAttackType > CONST_MAX_ATTACK_TYPE)
    {
        return 0;
    }
    return m_afAttack[iAttackType];
}

void CAttackValue::setAttack(ATTACK_TYPE eAttackType, float fAttack)
{
    if (eAttackType > CONST_MAX_ATTACK_TYPE)
    {
        return;
    }
    m_afAttack[eAttackType] = fAttack;
}

void CAttackValue::setAttack( const CAttackValue& roAv )
{
    memmove(m_afAttack, roAv.m_afAttack, sizeof(m_afAttack));
}

void CAttackValue::setAttack( int iAttackType, float fAttack )
{
    if (iAttackType > CONST_MAX_ATTACK_TYPE)
    {
        return;
    }
    m_afAttack[iAttackType] = fAttack;
}

void CAttackValue::setZero()
{
    memset(m_afAttack, 0, sizeof(m_afAttack));
}

const char* CAttackValue::getName(ATTACK_TYPE eAttackType, int iIndex)
{
    return CONST_ARR_NAME[eAttackType][iIndex];
}

const CAttackValue& CAttackValue::operator=( const CAttackValue& roAv )
{
    setAttack(roAv);
    return *this;
}

const char* CArmorValue::CONST_ARR_NAME[CONST_MAX_ARMOR_TYPE][CONST_MAX_NAME_INDEX] = {
    { "Normal", "普通" },
    { "Heavy", "重装" },
    { "Crystal", "水晶" },
    { "Wall", "城墙" },
    { "Hero", "英雄" },
    { "Holy", "神圣" }
};

CArmorValue::CArmorValue()
{
    setZero();
}

CArmorValue::CArmorValue( int iCount, ARMOR_TYPE eType1, float fValue1, ... )
{
    setZero();
    setArmor(eType1, fValue1);
    va_list argv;
    va_start(argv, fValue1);
    for (int i = 1; i < iCount; ++i)
    {
        eType1 = (ARMOR_TYPE)va_arg(argv, int);
        fValue1 = (float)va_arg(argv, double);
        setArmor(eType1, fValue1);
    }
    va_end(argv);
}

CArmorValue::~CArmorValue()
{
}

float CArmorValue::getArmor(ARMOR_TYPE eArmorType) const
{
    if (eArmorType > CONST_MAX_ARMOR_TYPE)
    {
        return 0;
    }
    return m_afArmor[eArmorType];
}

void CArmorValue::setArmor(ARMOR_TYPE eArmorType, float fArmor)
{
    if (eArmorType > CONST_MAX_ARMOR_TYPE)
    {
        return;
    }
    m_afArmor[eArmorType] = fArmor;
}

void CArmorValue::setArmor( const CArmorValue& roAv )
{
    memmove(m_afArmor, roAv.m_afArmor, sizeof(m_afArmor));
}

void CArmorValue::setZero()
{
    memset(m_afArmor, 0, sizeof(m_afArmor));
}

const char* CArmorValue::getName(ARMOR_TYPE eArmorType, int iIndex)
{
    return CONST_ARR_NAME[eArmorType][iIndex];
}

const CArmorValue& CArmorValue::operator=( const CArmorValue& roAv )
{
    setArmor((roAv));
    return *this;
}

/*
0.00    无
0.25    微弱
0.50    弱
0.75    较弱
1.00    正常
1.25    较强
1.50    强
1.75    超强
2.00    瓦解
*/

float g_afAttackArmorTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE] = {
//           物理攻击 魔法攻击 攻城攻击 神圣攻击
/*普通护甲*/ { 1.00,   1.00,   1.00,   1.00 },
/*重装护甲*/ { 0.50,   1.50,   0.75,   1.00 },
/*水晶护甲*/ { 1.25,   0.50,   1.50,   1.00 },
/*城墙护甲*/ { 0.50,   0.50,   1.50,   1.00 },
/*英雄护甲*/ { 0.75,   0.75,   0.75,   1.00 },
/*神圣护甲*/ { 0.25,   0.25,   0.25,   1.00 }
};

CAttackBuff::CAttackBuff()
    : m_pBuff(NULL)
    , m_iProbability(0)
{
}

CAttackBuff::~CAttackBuff()
{
    CC_SAFE_RELEASE(m_pBuff);
}

bool CAttackBuff::init(CBuffSkill* pBuff, int iProbability)
{
    m_pBuff = pBuff;
    CC_SAFE_RETAIN(m_pBuff);

    m_iProbability = iProbability;

    return true;
}

CAttackData::CAttackData()
    : m_fAngle(0)
{
}

CAttackData::~CAttackData()
{
    m_oArrBuff.removeAllObjects();
}

bool CAttackData::init()
{
    m_oArrBuff.init();
    return true;
}

void CAttackData::addBuff( CBuffSkill* pBuff, int iProbability )
{
    CAttackBuff* pAtkBuff = CAttackBuff::create(pBuff, iProbability);
    m_oArrBuff.addObject(pAtkBuff);
}

CExtraCoeff::CExtraCoeff()
    : m_fMulriple(1)
    , m_fAddend(0)
{

}

CExtraCoeff::CExtraCoeff(float fMulriple, float fAddend)
    : m_fMulriple(fMulriple)
    , m_fAddend(fAddend)
{
}

CExtraCoeff::~CExtraCoeff()
{

}

float CExtraCoeff::getValue(float fBase) const
{
    return fBase * m_fMulriple + m_fAddend;
}

CUnitForce::CUnitForce()
    : m_dwForceFlag(0)
    , m_dwAllyMaskFlag(0)
{
}

void CUnitForce::setForceByIndex( int iForceIndex )
{
    m_dwForceFlag = 1 << iForceIndex;
}

bool CUnitForce::isAllyOf( const CUnitForce* pForce ) const
{
    return ((m_dwAllyMaskFlag & pForce->m_dwForceFlag) != 0);
}

bool CUnitForce::isEnemyOf( const CUnitForce* pForce ) const
{
    return !isAllyOf(pForce);
}

bool CForceResouce::init()
{
    setGold(0);
    m_pSender = NULL;
    m_pCallback = NULL;
    return true;
}

bool CForceResouce::initWithChangeCallback( CCObject* pSender, SEL_CallFuncO pFunc )
{
    setGold(0);
    m_pSender = pSender;
    m_pCallback = pFunc;
    return true;
}

void CForceResouce::changeGold( int iChange )
{
    m_iGold += iChange;
    if (iChange)
    {
        onGoldChange(iChange);
    }
}

void CForceResouce::onGoldChange( int iChange )
{
    if (m_pSender && m_pCallback)
    {
        (m_pSender->*m_pCallback)(dynamic_cast<CCObject*>(this));
    }
}

// CGameUnit
CUnit::CUnit()
    : m_iKey(CGameManager::sharedGameManager()->keygen())
    , m_fStrength(0)
    , m_fIntellect(0)
    , m_fAgility(0)
    , m_fSpirit(0)
    , m_eArmorType((CArmorValue::ARMOR_TYPE)0)
    , m_iTriggering(0)
{

}


CUnit::~CUnit()
{
    //CCLOG("~CGameUnit()");
}

bool CUnit::init()
{
    m_oArrOnAttackTargetChain.init();
    m_oArrOnAttackedChain.init();
    m_oArrOnDamagedSurfaceChain.init();
    m_oArrOnDamagedInnerChain.init();
    m_oArrOnDamageTargetChain.init();
    m_oArrOnHpChangeChain.init();
    m_oArrOnReviveChain.init();
    m_oArrOnDieChain.init();
    m_oArrOnTickChain.init();
    m_oArrOnTickDelChain.init();
    m_oArrOnDestroyProjectileChain.init();
    m_oArrBuff.init();
    m_oArrSkill.init();
    return true;
}

CAttackData* CUnit::transformAttackByAttribute(CAttackData* pAttack)
{
    return pAttack;
}

CAttackData* CUnit::transformDamageByAttribute(CAttackData* pAttack)
{
    return pAttack;
}

CAttackData* CUnit::attackAdv(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    pAttack = transformAttackByAttribute(pAttack);
    return attackMid(pAttack, pTarget, dwTriggerMask);
}

CAttackData* CUnit::attackMid(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    onAttackTarget(pAttack, pTarget, dwTriggerMask);
    return attackBot(pAttack, pTarget, dwTriggerMask);
}

CAttackData* CUnit::attackBot(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    return pAttack;
}

void CUnit::damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    pAttack = onAttacked(pAttack, pSource, dwTriggerMask);
    if (!pAttack)
    {
        return;
    }
    damagedMid(pAttack, pSource, dwTriggerMask);
}

void CUnit::damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    onDamaged(pAttack, pSource, dwTriggerMask);

    CAttackBuff* pAtkBuff = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&pAttack->m_oArrBuff, pObj)
    {
        pAtkBuff = dynamic_cast<CAttackBuff*>(pObj);
        if (rand() % 100 < pAtkBuff->m_iProbability)
        {
            addBuff(pAtkBuff->m_pBuff);
        }
    }

    transformDamageByAttribute(pAttack);
    float fDamage = 0;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; i++)
    {
        fDamage += pAttack->getAttack((CAttackValue::ATTACK_TYPE)i) * g_afAttackArmorTable[m_eArmorType][i];
    }
    //CCLOG("%.2f", fDamage);

    damagedBot(fDamage, pSource, dwTriggerMask);
}

void CUnit::damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (pSource)
    {
        pSource->onDamageTarget(fDamage, this, dwTriggerMask);
    }

    if (fDamage > m_fHp)
    {
        setHp(0);
    }
    else
    {
        setHp(m_fHp - fDamage);
    }
}

void CUnit::onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask)
{
    if (isMasked(kAttackTargetTrigger, dwTriggerMask))
    {
        return;
    }
    triggerOnAttackTarget(pAttack, pTarget);
}

CAttackData* CUnit::onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (isMasked(kAttackedTrigger, dwTriggerMask))
    {
        return pAttack;
    }
    return triggerOnAttacked(pAttack, pSource);
}

void CUnit::onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    if (isMasked(kDamagedSurfaceTrigger, dwTriggerMask))
    {
        return;
    }
    triggerOnDamagedSurface(pAttack, pSource);

    if (isMasked(kDamagedInnerTrigger, dwTriggerMask))
    {
        return;
    }
    triggerOnDamagedInnerTrigger(pAttack, pSource);
}

void CUnit::onDamageTarget( float fDamage, CUnit* pTarget, uint32_t dwTriggerMask )
{
    if (isMasked(kDamageTargetTrigger, dwTriggerMask))
    {
        return;
    }
    triggerOnDamageTargetTrigger(fDamage, pTarget);
}

void CUnit::onRevive()
{
    triggerOnRevive();
}

void CUnit::onDie()
{
    triggerOnDie();
}

void CUnit::onHpChange( float fChanged )
{
    triggerOnHpChange(fChanged);
}

void CUnit::onTick(float fDt)
{
    triggerOnTick(fDt);
}

void CUnit::onDestroyProjectile( CCProjectileWithAttackData* pProjectile )
{
    triggerOnDestroyProjectileTrigger(pProjectile);
}

void CUnit::addToOnAttackTargetTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnAttackTargetChain, pSkill);
}

void CUnit::addToOnAttackedTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnAttackedChain, pSkill);
}

void CUnit::addToOnDamagedSurfaceTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnDamagedSurfaceChain, pSkill);
}

void CUnit::addToOnDamagedInnerTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnDamagedInnerChain, pSkill);
}

void CUnit::addToOnDamageTargetTrigger( CSkill* pSkill )
{
    CUnit::addToTrigger(m_oArrOnDamageTargetChain, pSkill);
}

void CUnit::addToOnHpChangeTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnHpChangeChain, pSkill);
}

void CUnit::addToOnReviveTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnReviveChain, pSkill);
}

void CUnit::addToOnDieTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnDieChain, pSkill);
}

void CUnit::addToOnTickTrigger(CSkill* pSkill)
{
    CUnit::addToTrigger(m_oArrOnTickChain, pSkill);
}

void CUnit::addToOnDestroyProjectileTrigger( CSkill* pSkill )
{
    CUnit::addToTrigger(m_oArrOnDestroyProjectileChain, pSkill);
}

void CUnit::delFromOnAttackTargetTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnAttackTargetChain, pSkill);
}

void CUnit::delFromOnAttackedTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnAttackedChain, pSkill);
}

void CUnit::delFromOnDamagedSurfaceTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnDamagedSurfaceChain, pSkill);
}

void CUnit::delFromOnDamagedInnerTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnDamagedInnerChain, pSkill);
}

void CUnit::delFromOnDamageTargetTrigger( CSkill* pSkill )
{
    CUnit::delFromTrigger(m_oArrOnDamageTargetChain, pSkill);
}

void CUnit::delFromOnHpChangeTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnHpChangeChain, pSkill);
}

void CUnit::delFromOnReviveTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnReviveChain, pSkill);
}

void CUnit::delFromOnDieTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnDieChain, pSkill);
}

void CUnit::delFromOnTickTrigger(CSkill* pSkill)
{
    CUnit::delFromTrigger(m_oArrOnTickChain, pSkill);
}

void CUnit::delFromOnDestroyProjectileTrigger( CSkill* pSkill )
{
    CUnit::delFromTrigger(m_oArrOnDestroyProjectileChain, pSkill);
}

void CUnit::delFromOnTickTriggerLater(CSkill* pSkill)
{
    m_oArrOnTickDelChain.addObject(pSkill);
}

void CUnit::addToTrigger(CCArray& roTrigger, CSkill* pSkill)
{
    roTrigger.addObject(pSkill);
}

void CUnit::delFromTrigger(CCArray& roTrigger, CSkill* pSkill)
{
    roTrigger.removeObject(pSkill);
}

void CUnit::triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnAttackTargetChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitAttackTarget(pAttack, pTarget);
    }
    endTrigger();
}

CAttackData* CUnit::triggerOnAttacked(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnAttackedChain, pObj)
    {
        pAttack = dynamic_cast<CSkill*>(pObj)->onUnitAttacked(pAttack, pSource);
    }
    endTrigger();
    return pAttack;
}

void CUnit::triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnDamagedSurfaceChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitDamaged(pAttack, pSource);
    }
    endTrigger();
}

void CUnit::triggerOnDamagedInnerTrigger(CAttackData* pAttack, CUnit* pSource)
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnDamagedInnerChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitDamaged(pAttack, pSource);
    }
    endTrigger();
}

void CUnit::triggerOnDamageTargetTrigger( float fDamage, CUnit* pTarget )
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnDamageTargetChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitDamageTarget(fDamage, pTarget);
    }
    endTrigger();
}

void CUnit::triggerOnHpChange( float fChanged )
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnHpChangeChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitHpChange(fChanged);
    }
    endTrigger();
}

void CUnit::triggerOnRevive()
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnReviveChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitRevive();
    }
    endTrigger();
}

void CUnit::triggerOnDie()
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnDieChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitDie();
    }
    endTrigger();
}

void CUnit::triggerOnTick(float fDt)
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnTickChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitTick(fDt);
    }
    if (m_oArrOnTickDelChain.count())
    {
        m_oArrOnTickChain.removeObjectsInArray(&m_oArrOnTickDelChain);
        m_oArrOnTickDelChain.removeAllObjects();
    }
    endTrigger();
}

void CUnit::triggerOnDestroyProjectileTrigger( CCProjectileWithAttackData* pProjectile )
{
    beginTrigger();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrOnDestroyProjectileChain, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitDestroyProjectile(pProjectile);
    }
    endTrigger();
}

void CUnit::addSkill( CSkill* pSkill )
{
    pSkill->setOwner(this);
    pSkill->onSkillAdd();
    m_oArrSkill.addObject(pSkill);   // will be instead by CCArray::addObject
}

void CUnit::delSkill( CSkill* pSkill )
{
    pSkill->onSkillDel();
    pSkill->setOwner(NULL);
    m_oArrSkill.removeObject(pSkill);   // will be instead by CCArray::addObject
}

void CUnit::addPackage(CUnitPackage* pPackage)
{
	pPackage->setOwner(this);
	m_pUnitPackage = pPackage;

}
void CUnit::addBuff( CBuffSkill* pBuff, bool bForce )
{
    pBuff->setOwner(this);
    if (!pBuff->canBePlural() && !bForce)
    {
        CBuffSkill* pOldBuff = getBuffByType(pBuff->getTypeKey());
        if (pOldBuff)
        {
            delBuff(pOldBuff, false);
            //pOldBuff->setDuration(pOldBuff->getDuration() + pBuff->getDuration());
        }
    }
    
    pBuff->onSkillAdd();
    m_oArrBuff.addObject(pBuff);
}

void CUnit::delBuff( CBuffSkill* pBuff, bool bAfterTriggerLoop )
{
    pBuff->onSkillDel();
    pBuff->setOwner(NULL);
    m_oArrBuff.removeObject(pBuff);
    if (!bAfterTriggerLoop)
    {
        m_oArrOnTickChain.removeObjectsInArray(&m_oArrOnTickDelChain);
        m_oArrOnTickDelChain.removeAllObjects();
    }
}

CSkill* CUnit::getSkill( int iKey )
{
    CSkill* pSkill = NULL;

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrSkill, pObj)
    {
        pSkill = dynamic_cast<CSkill*>(pObj);
        if (pSkill->getKey() == iKey)
        {
            return pSkill;
        }
    }
    return NULL;
}

CBuffSkill* CUnit::getBuff( int iKey )
{
    CBuffSkill* pBuff = NULL;

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrBuff, pObj)
    {
        pBuff = dynamic_cast<CBuffSkill*>(pObj);
        if (pBuff->getKey() == iKey)
        {
            return pBuff;
        }
    }
    return NULL;
}

CBuffSkill* CUnit::getBuffByType( int iTypeKey )
{
    CBuffSkill* pBuff = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrBuff, pObj)
    {
        pBuff = dynamic_cast<CBuffSkill*>(pObj);
        if (pBuff->getTypeKey() == iTypeKey)
        {
            return pBuff;
        }
    }

    return NULL;
}

void CUnit::cleanUpTriggers()
{
    CCArray oArrCopy;
    oArrCopy.initWithArray(&m_oArrBuff);
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&oArrCopy, pObj)
    {
        delBuff(dynamic_cast<CBuffSkill*>(pObj), false);
    }
    
    oArrCopy.initWithArray(&m_oArrSkill);
    CCARRAY_FOREACH(&oArrCopy, pObj)
    {
        delSkill(dynamic_cast<CSkill*>(pObj));
    }

    oArrCopy.removeAllObjects();
    m_oArrBuff.removeAllObjects();
    m_oArrSkill.removeAllObjects();
    m_oArrOnAttackTargetChain.removeAllObjects();
    m_oArrOnAttackedChain.removeAllObjects();
    m_oArrOnDamagedSurfaceChain.removeAllObjects();
    m_oArrOnDamagedInnerChain.removeAllObjects();
    m_oArrOnDamageTargetChain.removeAllObjects();
    m_oArrOnHpChangeChain.removeAllObjects();
    m_oArrOnReviveChain.removeAllObjects();
    m_oArrOnDieChain.removeAllObjects();
    m_oArrOnTickChain.removeAllObjects();
    m_oArrOnTickDelChain.removeAllObjects();
    m_oArrOnDestroyProjectileChain.removeAllObjects();
}

void CUnit::beginTrigger()
{
    ++m_iTriggering;
}

void CUnit::endTrigger()
{
    --m_iTriggering;
    CCAssert(m_iTriggering >= 0, "");
    if (isDead() && !m_iTriggering)
    {
        cleanUpTriggers();
    }
}

bool CUnit::isTriggerFree() const
{
    return m_iTriggering == 0;
}

int CUnit::getKey() const
{
    return m_iKey;
}

CCGameUnitSprite::CCGameUnitSprite()
{
}

CCGameUnitSprite::~CCGameUnitSprite()
{
    //CC_SAFE_RELEASE(m_pControler);
}

bool CCGameUnitSprite::init()
{
    bool bRes = CCSprite::init();
    updateShadowNode();
    return bRes;
}

bool CCGameUnitSprite::initWithSpriteFrame( CCSpriteFrame *pSpriteFrame, const CCPoint& roAnchor )
{
    bool bRes = CCSprite::initWithSpriteFrame(pSpriteFrame);
    setAnchorPoint(roAnchor);
    updateShadowNode();
    return bRes;
}

void CCGameUnitSprite::updateShadowNode()
{
    m_oShadowNode.setAnchorPoint(getAnchorPoint());
    m_oShadowNode.setContentSize(getContentSize());
    m_oShadowNode.setPosition(getPosition());
}

void CCGameUnitSprite::setDisplayFrame( CCSpriteFrame *pNewFrame )
{
    CCSprite::setDisplayFrame(pNewFrame);
    updateShadowNode();
}

void CCGameUnitSprite::setPosition( const CCPoint& roPos )
{
    CCNode* pLayer = m_oShadowNode.getParent();
    int iZ = 10000 - roPos.y + m_pControler->getOffsetZ();
    bool bZ = m_oShadowNode.getZOrder() != iZ;
    if (pLayer)
    {
        m_oShadowNode.setPosition(roPos);
        if (bZ)
        {
            pLayer->reorderChild(&m_oShadowNode, iZ);
        }
        
    }

    pLayer = getParent();
    if (pLayer && bZ)
    {
        pLayer->reorderChild(this, iZ);
    }
    CCSprite::setPosition(roPos);
    
}

CCNode* CCGameUnitSprite::getShadowNode()
{
    return &m_oShadowNode;
}

void CCGameUnitSprite::draw()
{
    CCSprite::draw();return;
    CCPoint oPos = getAnchorPoint();
    M_DEF_DR(pDr);
    //oPos = pDr->convertToGL(oPos);
    //ccDrawLine(ccp(0, oPos.y), ccp(800, oPos.y));
    float w = getControler()->getHalfOfWidth() * 2;
    float h = getControler()->getHalfOfHeight() * 2;
    CCSize oSz = getContentSize();
    ccDrawColor4B(255, 255, 255, 255);
    ccDrawRect(ccp(0, 0), ccp(oSz.width, oSz.height));
    ccDrawColor4B(255, 0, 0, 255);
    ccDrawLine(ccp((oSz.width - w) * 0.5, oPos.y * oSz.height), ccp((oSz.width + w) * 0.5, oPos.y * oSz.height));
    ccDrawLine(ccp(oPos.x * oSz.width, oPos.y * oSz.height), ccp(oPos.x * oSz.width, oPos.y * oSz.height + h));
    
}

const float CGameUnit::CONST_MIN_MOVE_SPEED = 1.0;
const float CGameUnit::CONST_MAX_MOVE_SPEED = 500.0;
const float CGameUnit::CONST_MIN_MOVE_SPEED_MULRIPLE = 0.2; // 最小变为基础速度的20%

const float CGameUnit::CONST_MIN_ATTACK_SPEED_INTERVAL = 0.1; // 0.1s
const float CGameUnit::CONST_MIN_ATTACK_SPEED_MULRIPLE = 0.2; // 20%
const float CGameUnit::CONST_MAX_ATTACK_SPEED_MULRIPLE = 5.0; // 500%
const float CGameUnit::CONST_MAX_ATTACK_BUFFER_RANGE = 50.0;
const float CGameUnit::CONST_MAX_CLOSE_ATTACK_Y_RANGE = 5.0;

const float CGameUnit::CONST_MAX_REWARD_RANGE = 400;

CGameUnit::CGameUnit()
{
}

bool CGameUnit::init()
{
    m_oSprite.init();
    m_oSprite.setControler(this);
    m_dwDoingFlags = 0;

    setBaseMoveSpeed(0);

    setAttackRange(0);
    setAttackMinRange(0);
    setBaseAttackInterval(0);
    setAttackCD(0);
    m_iLastAttackTarget = 0;
    setHalfOfWidth(0);
    setHalfOfHeight(0);
    setOffsetZ(0);
    setTemplateProjectile(NULL);
    setProjectileMoveSpeed(0);
    setProjectileScale(1);
    setProjectileMaxOffsetY(0);
    setAttackEffectDelay(0);
    setFixed(false);
    setHostilityRange(0);
    setProjectileBirthOffsetX(0);
    setProjectileBirthOffsetY(0);
    setRewardGold(0);
    setRewardExp(0);
    setExAttackRandomRange(0.000);
    m_pRes = NULL;

    return true;
}

bool CGameUnit::initWithName( const char* pUnit, const CCPoint& roAnchor )
{
    setName(pUnit, roAnchor);
    m_oSprite.setControler(this);
    m_dwDoingFlags = 0;

    setBaseMoveSpeed(0);

    setAttackRange(0);
    setAttackMinRange(0);
    setBaseAttackInterval(0);
    setAttackCD(0);
    m_iLastAttackTarget = 0;
    setOffsetZ(0);
    setTemplateProjectile(NULL);
    setProjectileMoveSpeed(0);
    setProjectileScale(1);
    setProjectileMaxOffsetY(0);
    setAttackEffectDelay(0);
    setFixed(false);
    setHostilityRange(0);
    setProjectileBirthOffsetX(0);
    setProjectileBirthOffsetY(0);
    setRewardGold(0);
    setRewardExp(0);
    setExAttackRandomRange(0.000);
    m_pRes = NULL;

    return true;
}

bool CGameUnit::initWithInfo( const CUnitInfo& roUnitInfo )
{
    M_DEF_PM(pPm);
    CGameUnit::initWithName(roUnitInfo.m_sName.c_str(), roUnitInfo.m_oAnchor);
    setHalfOfWidth(roUnitInfo.m_fHalfOfWidth);
    setHalfOfHeight(roUnitInfo.m_fHalfOfHeight);
    m_oSprite.setScale(roUnitInfo.m_fScale);
    if (roUnitInfo.m_fActMoveDelay)
    {
        prepareMoveAnimation("move", roUnitInfo.m_fActMoveDelay);
    }
    if (roUnitInfo.m_fActDieDelay)
    {
        prepareDieAnimation("die", roUnitInfo.m_fActDieDelay);
    }
    if (roUnitInfo.m_fAct1Delay)
    {
        prepareAnimation(kAnimationAct1, "act1", roUnitInfo.m_fAct1Delay);
    }
    if (roUnitInfo.m_fAct2Delay)
    {
        prepareAnimation(kAnimationAct2, "act2", roUnitInfo.m_fAct2Delay);
    }
    if (roUnitInfo.m_fAct3Delay)
    {
        prepareAnimation(kAnimationAct3, "act3", roUnitInfo.m_fAct3Delay);
    }
    if (roUnitInfo.m_fAct4Delay)
    {
        prepareAnimation(kAnimationAct4, "act4", roUnitInfo.m_fAct4Delay);
    }
    if (roUnitInfo.m_fAct5Delay)
    {
        prepareAnimation(kAnimationAct5, "act5", roUnitInfo.m_fAct5Delay);
    }
    if (roUnitInfo.m_fAct6Delay)
    {
        prepareAnimation(kAnimationAct6, "act6", roUnitInfo.m_fAct6Delay);
    }
    m_vecAttackAniIndex = roUnitInfo.m_vecAttackAni;

    setBaseMoveSpeed(roUnitInfo.m_fBaseMoveSpeed);
    setBaseAttackInterval(roUnitInfo.m_fBaseAttackInterval);
    setAttackEffectDelay(roUnitInfo.m_fAttackEffectDelay);
    setAttackMinRange(roUnitInfo.m_fAttackMinRange);
    setAttackRange(roUnitInfo.m_fAttackRange);
    setHostilityRange(roUnitInfo.m_fHostilityRange);
    setWeaponType(roUnitInfo.m_eWeaponType);
    setTemplateProjectile(pPm->getProjectile(roUnitInfo.m_iProjectileKey));
    setProjectileMoveSpeed(roUnitInfo.m_fProjectileMoveSpeed);
    setProjectileScale(roUnitInfo.m_fProjectileScale);
    setProjectileMaxOffsetY(roUnitInfo.m_fProjectileMaxOffsetY);
    setProjectileBirthOffsetX(roUnitInfo.m_fProjectileBirthOffsetX);
    setProjectileBirthOffsetY(roUnitInfo.m_fProjectileBirthOffsetY);
    setBaseAttackValue(roUnitInfo.m_oBaseAttackValue);
    setExAttackRandomRange(roUnitInfo.m_fExAttackRandomRange);
    setArmorType(roUnitInfo.m_eArmorType);
    setBaseArmorValue(roUnitInfo.m_fBaseArmorValue);
    setForceByIndex(roUnitInfo.m_iForceIndex);
    setAlly(roUnitInfo.m_dwForceAlly);
    setMaxHp(roUnitInfo.m_fMaxHp);
    setFixed(roUnitInfo.m_bIsFixed);
    setRewardGold(roUnitInfo.m_iRewardGold);
    setRewardExp(roUnitInfo.m_iRewardExp);
    return true;
}

void CGameUnit::setName( const char* pUnit, const CCPoint& roAnchor )
{
    m_sUnit = pUnit;
    m_oSprite.initWithSpriteFrame(CGameManager::sharedGameManager()->getUnitFrame(m_sUnit.c_str(), NULL), roAnchor);
    setHalfOfWidth(m_oSprite.getContentSize().width * 0.5);
    setHalfOfHeight(m_oSprite.getContentSize().height * 0.5);
}

const char* CGameUnit::getName() const
{
    return m_sUnit.c_str();
}

void CGameUnit::setFrame( const char* pFrame )
{
    m_oSprite.setDisplayFrame(CGameManager::sharedGameManager()->getUnitFrame(m_sUnit.c_str(), pFrame));
}

void CGameUnit::setDefaultFrame()
{
    setFrame(NULL);
}

void CGameUnit::prepareMoveAnimation( const char* pAnimation, float fDelay )
{
    prepareAnimation(kAnimationMove, pAnimation, fDelay);
}

void CGameUnit::prepareAttackAnimation( int iAttackAniCount, ANIMATION_INDEX eAnimation1, const char* pAnimation1, float fDelay1, ... )
{
    m_vecAttackAniIndex.resize(iAttackAniCount);

    prepareAnimation(eAnimation1, pAnimation1, fDelay1);
    m_vecAttackAniIndex[0] = eAnimation1;

    va_list argv;
    va_start(argv, fDelay1);
    for (int i = 1; i < iAttackAniCount; ++i)
    {
        eAnimation1 = (ANIMATION_INDEX)va_arg(argv, int);
        pAnimation1 = (const char*)va_arg(argv, int);
        fDelay1 = (float)va_arg(argv, double);
        prepareAnimation(eAnimation1, pAnimation1, fDelay1);
        m_vecAttackAniIndex[i] = eAnimation1;
    }

    va_end(argv);
}

void CGameUnit::prepareDieAnimation( const char* pAnimation, float fDelay )
{
    prepareAnimation(kAnimationDie, pAnimation, fDelay);
}

void CGameUnit::prepareAnimation( ANIMATION_INDEX eAnimation, const char* pAnimation, float fDelay )
{
    ARR_ANIMATION_INFO& rAni = m_astAniInfo[eAnimation];
    rAni.sAnimation = pAnimation;
    rAni.fDelay = fDelay;
}

void CGameUnit::setAnimation( const char* pAnimation, float fDelay, int iRepeat, float fSpeed, ACTION_TAG eTag, CCFiniteTimeAction* pEndAction )
{
    if (m_oSprite.getActionByTag(eTag))
    {
        return;
    }
    M_DEF_GM(pGm);
    CCAnimation* pAni = pGm->getUnitAnimation(getName(), pAnimation);
    pAni->setDelayPerUnit(fDelay);
    CCAction* pAct = CCAnimate::create(pAni);
    
    switch (iRepeat)
    {
    case 0:
    case 1:
        pAct = CCSequence::create(dynamic_cast<CCAnimate*>(pAct), pEndAction, NULL);
        break;

    case INFINITE:
        pAct = CCRepeatForever::create(dynamic_cast<CCAnimate*>(pAct));
        break;

    default:
        pAct = CCSequence::create(CCRepeat::create(dynamic_cast<CCAnimate*>(pAct), iRepeat), pEndAction, NULL);
    }

    pAct = CCSpeed::create(dynamic_cast<CCActionInterval*>(pAct), fSpeed);
    pAct->setTag(eTag);
    m_oSprite.runAction(pAct);
}

void CGameUnit::setAnimation( ANIMATION_INDEX eAnimation, int iRepeat, float fSpeed, ACTION_TAG eTag, CCFiniteTimeAction* pEndAction )
{
    ARR_ANIMATION_INFO& rAni = m_astAniInfo[eAnimation];
    setAnimation(rAni.sAnimation.c_str(), rAni.fDelay, iRepeat, fSpeed, eTag, pEndAction);
}

void CGameUnit::setBaseMoveSpeed( float fMoveSpeed )
{
    fMoveSpeed = MAX(fMoveSpeed, CONST_MIN_MOVE_SPEED);
    fMoveSpeed = MIN(fMoveSpeed, CONST_MAX_MOVE_SPEED);
    updateMoveToAnimationSpeed(fMoveSpeed);
    m_fBaseMoveSpeed = fMoveSpeed;
}

float CGameUnit::getBaseMoveSpeed() const
{
    return m_fBaseMoveSpeed;
}

float CGameUnit::getRealMoveSpeed() const
{
    float fMoveSpeed = getBaseMoveSpeed();
    float fRealMoveSpeed = m_oExMoveSpeed.getValue(fMoveSpeed);
    // 取最小移动速度和最小减速后速度的最大值作为最小移动速度
    float fMinMoveSpeed = fMoveSpeed * CONST_MIN_MOVE_SPEED_MULRIPLE;
    fMinMoveSpeed = MAX(CONST_MIN_MOVE_SPEED, fMinMoveSpeed);
    // 计算实际移动速度，不得超过上述计算所得的最小值
    fRealMoveSpeed = MAX(fRealMoveSpeed, fMinMoveSpeed);
    // 计算实际移动速度，不得超过最大移动速度
    return MIN(fRealMoveSpeed, CONST_MAX_MOVE_SPEED);
}

void CGameUnit::setExMoveSpeed( const CExtraCoeff& roExMoveSpeed )
{
    m_oExMoveSpeed = roExMoveSpeed;
    updateMoveToAnimationSpeed(getRealMoveSpeed());
}

CExtraCoeff CGameUnit::getExMoveSpeed() const
{
    return m_oExMoveSpeed;
}

void CGameUnit::updateMoveToAnimationSpeed(float fRealMoveSpeed)
{
    CCSpeed* pAct = dynamic_cast<CCSpeed*>(m_oSprite.getActionByTag(kActMoveTo));
    if (!pAct)
    {
        return;
    }
    float fMoveSpeed = getBaseMoveSpeed();
    bool bOk = fMoveSpeed > FLT_EPSILON;
    if (!bOk)
    {
        stopMove();
        return;
    }
    float fDelta = fRealMoveSpeed / fMoveSpeed;
    pAct->setSpeed(fDelta);
    pAct = dynamic_cast<CCSpeed*>(m_oSprite.getActionByTag(kActMove));
    if (!pAct)
    {
        return;
    }
    pAct->setSpeed(fDelta);
}

void CGameUnit::moveTo( const CCPoint& roPos, bool bWithHostility /*= false*/, bool bCancelAttack /*= true*/, bool bAutoFlipX )
{
    if (isDead() || isFixed())
    {
        return;
    }
    m_oLastMoveToTarget = roPos;
    if (isDoingOr(kSuspended))
    {
        return;
    }
    M_DEF_GM(pGm);
    CCPoint roOrg = getPosition();
    if (bAutoFlipX)
    {
        turnTo(roOrg.x > roPos.x);
    }
    
    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = pGm->getDistance(roOrg, roPos) / fMoveSpeed;
    CCActionInterval* pSeq = CCSequence::createWithTwoActions(
        CCMoveTo::create(fDur, roPos),
        CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActMoveEnd))
        );
    float fDelta = getRealMoveSpeed() / fMoveSpeed;
    CCSpeed* pActMoveTo = CCSpeed::create(pSeq, fDelta);
    pActMoveTo->setTag(kActMoveTo);
    // 突发移动指令，打断旧移动，打断攻击
    if (isDoingOr(kMoving))
    {
        m_oSprite.stopActionByTag(kActMoveTo);
    }
    if (isDoingOr(kAttacking) && bCancelAttack)
    {
        stopAttack();
    }
    m_oSprite.runAction(pActMoveTo);
    setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    startDoing(kMoving);
    if (bWithHostility)
    {
        startDoing(kWithHostility);
    }
    else
    {
        endDoing(kWithHostility);
    }
}

void CGameUnit::followTo( int iTargetKey, bool bWithHostility /*= false*/, bool bCancelAttack /*= true*/, bool bAutoFlipX /*= true*/, float fMaxOffsetY /*= 0.0*/ )
{
    if (isDead() || isFixed())
    {
        return;
    }
    M_DEF_GM(pGm);
    CGameUnit* pTarget = pGm->getUnitByKey(iTargetKey);
    if (!pTarget)
    {
        return;
    }
    const CCPoint& roPos = pTarget->getPosition();
    m_oLastMoveToTarget = roPos;
    if (isDoingOr(kSuspended))
    {
        return;
    }
    CCPoint roOrg = getPosition();
    if (bAutoFlipX)
    {
        turnTo(roOrg.x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = pGm->getDistance(roOrg, roPos) / fMoveSpeed;
    CCActionInterval* pSeq = CCSequence::createWithTwoActions(
        CCMoveToNode::create(fDur, pTarget->getSprite(), fMaxOffsetY, 1, pTarget->getHalfOfHeight()),
        CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActMoveEnd))
        );
    float fDelta = getRealMoveSpeed() / fMoveSpeed;
    CCSpeed* pActMoveTo = CCSpeed::create(pSeq, fDelta);
    pActMoveTo->setTag(kActMoveTo);
    // 突发移动指令，打断旧移动，打断攻击
    if (isDoingOr(kMoving))
    {
        m_oSprite.stopActionByTag(kActMoveTo);
    }
    if (isDoingOr(kAttacking) && bCancelAttack)
    {
        stopAttack();
    }
    m_oSprite.runAction(pActMoveTo);
    setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    startDoing(kMoving);
    if (bWithHostility)
    {
        startDoing(kWithHostility);
    }
    else
    {
        endDoing(kWithHostility);
    }
}

void CGameUnit::stopMove()
{
    m_oSprite.stopActionByTag(kActMoveTo);
    m_oSprite.stopActionByTag(kActMove);
    endDoing(kMoving | kWithHostility);
    setDefaultFrame();
}

const CCPoint& CGameUnit::getLastMoveToTarget() const
{
    return m_oLastMoveToTarget;
}

void CGameUnit::onActMoveEnd( CCNode* pNode )
{
    stopMove();
}

void CGameUnit::setBaseAttackInterval( float fAttackInterval )
{
    m_fBaseAttackInterval = MAX(fAttackInterval, CONST_MIN_ATTACK_SPEED_INTERVAL);
}

float CGameUnit::getBaseAttackInterval() const
{
    return m_fBaseAttackInterval;
}

float CGameUnit::getBaseAttackSpeed() const
{
    return 1 / getBaseAttackInterval();
}

float CGameUnit::getRealAttackSpeed() const
{
    return 1 / getRealAttackInterval();
}

float CGameUnit::getRealAttackInterval() const
{
    float fAttackInterval = getBaseAttackInterval();
    // 取攻击速度系数，不得小于最小值
    float fMulriple = m_oExAttackSpeed.getMulriple();
    fMulriple = MAX(fMulriple, CONST_MIN_ATTACK_SPEED_MULRIPLE);
    // 计算两种最短攻击间隔中的最大值作为最短攻击间隔
    float fMinAttacSpeedInterval = fAttackInterval / CONST_MAX_ATTACK_SPEED_MULRIPLE;
    fMinAttacSpeedInterval = MAX(CONST_MIN_ATTACK_SPEED_INTERVAL, fMinAttacSpeedInterval);
    // 计算实际攻击间隔，不得小于上述最短攻击间隔
    float fRealAttackInterval = fAttackInterval / fMulriple;
    return MAX(fRealAttackInterval, fMinAttacSpeedInterval);
}

void CGameUnit::setExAttackSpeed( const CExtraCoeff& roExAttackSpeed )
{
    m_oExAttackSpeed = roExAttackSpeed;
    updateAttackAnimationSpeed(getRealAttackInterval());
}

CExtraCoeff CGameUnit::getExAttackSpeed() const
{
    return m_oExAttackSpeed;
}

void CGameUnit::updateAttackAnimationSpeed( float fRealAttackInterval )
{
    CCSpeed* pAct = dynamic_cast<CCSpeed*>(m_oSprite.getActionByTag(kActAttack));
    if (!pAct)
    {
        return;
    }
    float fAttackInterval = getBaseAttackInterval();
    CCAssert(fAttackInterval > FLT_EPSILON, "");
    float fDelta = fAttackInterval / fRealAttackInterval;
    pAct->setSpeed(fDelta);
    CCSequenceEx* pActSeq = dynamic_cast<CCSequenceEx*>(m_oSprite.getActionByTag(kActAttackEffect));
    if (!pActSeq)
    {
        return;
    }
    float fDur = getAttackEffectDelay() / fDelta;
    pActSeq->setDuration(fDur);
    dynamic_cast<CCDelayTime*>(pActSeq->getActionOne())->setDuration(fDur);
}

void CGameUnit::attack( int iTargetKey, bool bAuto /*= false*/)
{
    M_DEF_GM(pGm);
    if (isDead() || iTargetKey == getKey())
    {
        return;
    }
    float fRealAttackInterval = getRealAttackInterval();
    //M_DEF_GM(pGm);
    CGameUnit* pTarget = pGm->getUnitByKey(iTargetKey);
    if (!pTarget || pTarget->isDead())
    {
        if (isDoingOr(kAttacking) && !m_oSprite.getActionByTag(kActAttack))
        {
            //stopAttack();
            endDoing(kAttacking | kAutoAttack);
            m_iLastAttackTarget = 0;
        }
        return;
    }

    if (isDoingOr(kSuspended))
    {
        // 眩晕中，就退出
        //m_iLastAttackTarget = iTargetKey;
        return;
    }
    
    CCPoint roPos1 = getPosition();
    CCPoint roPos2 = pTarget->getPosition();
    //CCLOG("dis: %.2f, %.2f, %.2f | %.2f", pGm->getDistance(roPos1, roPos2) - getHalfOfWidth() - pTarget->getHalfOfWidth(), getAttackMinRange(), getAttackRange(), abs(roPos1.y - roPos2.y));
    if (checkAttackDistance(roPos1, pTarget))
    {
        // 可以将对该目标进行攻击

        // 位置符合，可以立即发动攻击，突发攻击指令，打断移动，打断旧攻击
        if (isDoingOr(kMoving))
        {
            stopMove();
        }

        if (getLastAttackTarget() == iTargetKey && m_oSprite.getActionByTag(kActAttack))
        {
            // 新攻击目标就是正在攻击着的目标，直接返回
            return;
        }

        if (isDoingOr(kAttacking))
        {
            stopAttack();
        }

        m_iLastAttackTarget = iTargetKey;
        turnTo(pTarget);

        if (m_fAttackCD > 0)
        {
            startDoing(kAttacking);
            endDoing(kAutoAttack);
            return;
        }

        // 发动攻击动作
        m_fAttackCD = fRealAttackInterval;
        startDoing(kAttacking);
        endDoing(kAutoAttack);
        float fDelta = getBaseAttackInterval() / fRealAttackInterval;
        setAnimation(m_vecAttackAniIndex[rand() % m_vecAttackAniIndex.size()], 0, fDelta, kActAttack, CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActAttackEnd)));
        CCAction* pAct = CCSequenceEx::createWithTwoActions(CCDelayTime::create(getAttackEffectDelay() / fDelta), CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActAttackEffect)));
        pAct->setTag(kActAttackEffect);
        m_oSprite.runAction(pAct);
        return;
    }

    // 由于位置原因，无法进行攻击
    if (isFixed())
    {
        // 如果是建筑，只需丢失攻击目标即可
        m_iLastAttackTarget = 0;
        endDoing(kAttacking | kAutoAttack);
        return;
    }

    if (getLastAttackTarget() == iTargetKey)
    {
        if (isDoingAnd(kAttacking | kMoving))
        {
            // 位置校正中
            if (!checkAttackDistance(getLastMoveToTarget(), pTarget))
            {
                // 攻击目标未变，但修正位置已过期
                //if (getWeaponType() == kWTDelayed) CCLOG("!!!!!!!!!");
                moveToAttackPosition(pTarget, bAuto);
            }
            return;
        }

        if (m_oSprite.getActionByTag(kActAttack) || m_oSprite.getActionByTag(kActAttackEffect))
        {
            return;
        }
    }
    
    m_iLastAttackTarget = iTargetKey;
    moveToAttackPosition(pTarget, bAuto);
}

void CGameUnit::stopAttack()
{
    m_oSprite.stopActionByTag(kActAttack);
    m_oSprite.stopActionByTag(kActAttackEffect);
    m_iLastAttackTarget = 0;
    endDoing(kAttacking | kAutoAttack);
    setDefaultFrame();
}

void CGameUnit::onActAttackEffect( CCNode* pNode )
{
    CAttackData* pAtk = CAttackData::create();
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        pAtk->setAttack((CAttackValue::ATTACK_TYPE)i, getRealAttackValue((CAttackValue::ATTACK_TYPE)i));
    }
    M_DEF_GM(pGm);
    CGameUnit* pTarget = pGm->getUnitByKey(getLastAttackTarget());
    if (!pTarget || pTarget->isDead())
    {
        return;
    }
    CProjectile* pProj;
    if (!pTarget)
    {
        return;
    }
    pAtk = attackAdv(pAtk, pTarget);
    if (getDistance(pTarget) > getAttackRange() + getHalfOfWidth() + pTarget->getHalfOfWidth() + CONST_MAX_ATTACK_BUFFER_RANGE)
    {
        return;
    }
    switch (getWeaponType())
    {
    case kWTClose:
    case kWTInstant:
        if (!getTemplateProjectile())
        {
            pTarget->damagedAdv(pAtk, this);
        }
        else
        {
            pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
            CCUnitLayer* pLayer = dynamic_cast<CCUnitLayer*>(m_oSprite.getParent());
            pLayer->addProjectile(pProj);
            pProj->setAttackData(pAtk);
            pProj->setOwner(getKey());
            pProj->setTarget(getLastAttackTarget());
            pProj->getSprite()->setScale(getProjectileScale());
            pProj->setPosition(pTarget->getPosition());
            pProj->onDie();
        }
        break;

    case kWTDelayed:
        pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
        CCUnitLayer* pLayer = dynamic_cast<CCUnitLayer*>(m_oSprite.getParent());
        pLayer->addProjectile(pProj);
        pProj->setAttackData(pAtk);
        pProj->setOwner(getKey());
        pProj->setTarget(getLastAttackTarget());
        pProj->setBaseMoveSpeed(getProjectileMoveSpeed());
        const CCPoint& roPos1 = pProj->getPosition();
        const CCPoint& roPos2 = pTarget->getPosition();
        float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(roPos2, roPos1)));
        pProj->getSprite()->setScale(getProjectileScale());
        pProj->getSprite()->setRotation(fA);
        float fOffsetX = getHalfOfWidth();
        pProj->setPosition(ccpAdd(getPosition(), ccp(getSprite()->isFlipX() ? -getProjectileBirthOffsetX() : getProjectileBirthOffsetX(), getProjectileBirthOffsetY())));
        //pProj->setHalfOfWidth(11);
        //pProj->setHalfOfHeight(13);
        pProj->followTo(getLastAttackTarget(), false, true, false, getProjectileMaxOffsetY());

        break;

    }
}

void CGameUnit::onActAttackEnd( CCNode* pNode )
{
    //stopAttack(); // use this later
    setDefaultFrame();
}

void CGameUnit::setBaseAttackValue( const CAttackValue& roAttackValue )
{
    m_oBaseAttackValue = roAttackValue;
}

float CGameUnit::getBaseAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const
{
    return m_oBaseAttackValue.getAttack(eAttackType);
}

float CGameUnit::getRealAttackValue( CAttackValue::ATTACK_TYPE eAttackType, bool bUseRandomRange /*= true*/ ) const
{
    if (bUseRandomRange)
    {
        //CCLOG("Atk: %.2f", m_aoExAttackValue[eAttackType].getValue(m_oBaseAttackValue.getAttack(eAttackType)) * (1 - m_fExAttackRandomRange + (rand() % (int)(m_fExAttackRandomRange * 2000.0)) / 1000.0));
        return m_aoExAttackValue[eAttackType].getValue(m_oBaseAttackValue.getAttack(eAttackType)) * (1 - m_fExAttackRandomRange + (rand() % (int)(m_fExAttackRandomRange * 2000.0)) / 1000.0);
    }
    return m_aoExAttackValue[eAttackType].getValue(m_oBaseAttackValue.getAttack(eAttackType));
}

void CGameUnit::setExAttackValue( CAttackValue::ATTACK_TYPE eAttackType, const CExtraCoeff& roExAttackValue )
{
    m_aoExAttackValue[eAttackType] = roExAttackValue;
}

CExtraCoeff CGameUnit::getExAttackValue( CAttackValue::ATTACK_TYPE eAttackType ) const
{
    return m_aoExAttackValue[eAttackType];
}

int CGameUnit::getLastAttackTarget() const
{
    return m_iLastAttackTarget;
}

bool CGameUnit::checkAttackDistance( const CCPoint& roPos, CGameUnit* pTarget )
{
    M_DEF_GM(pGm);
    const CCPoint& roPos2 = pTarget->getPosition();
    //CCLOG("dis: %.2f, %.2f, %.2f | %.2f", pGm->getDistance(roPos, roPos2) - getHalfOfWidth() - pTarget->getHalfOfWidth(), getAttackMinRange(), getAttackRange(), abs(roPos.y - roPos2.y));
    if (getWeaponType() == kWTClose && abs(roPos.y - roPos2.y) > CONST_MAX_CLOSE_ATTACK_Y_RANGE)
    {
        return false;
    }

    float fDis = pGm->getDistance(roPos, roPos2) - getHalfOfWidth() - pTarget->getHalfOfWidth();
    fDis = MAX(0.5, fDis);
    if (fDis < getAttackMinRange() || fDis > getAttackRange())
    {
        return false;
    }

    return true;
}

void CGameUnit::moveToAttackPosition( CGameUnit* pTarget, bool bAuto )
{
    if (isFixed())
    {
        return;
    }
    m_oSprite.stopActionByTag(kActAttack);
    m_oSprite.stopActionByTag(kActAttackEffect);
    float fDis = pTarget->getHalfOfWidth() + getHalfOfWidth() + (getAttackMinRange() + getAttackRange()) * 0.5;
    const CCPoint& roPos1 = getPosition();
    const CCPoint& roPos2 = pTarget->getPosition();
    if (getWeaponType() == kWTClose)
    {
        // 近战攻击位置修正
        moveTo(ccp(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), false, false);
    }
    else
    {
        // 远程攻击位置修正
        float fA = -ccpToAngle(ccpSub(roPos1, roPos2));
        moveTo(ccpAdd(roPos2, ccp(cos(fA) * fDis, sin(-fA) * fDis)), false, false);
    }
    startDoing(kAttacking);
    startDoing(bAuto ? kAutoAttack : 0);
}

void CGameUnit::setTemplateProjectile( CProjectile* pProjectile )
{
    m_pTemplateProjectile = pProjectile;
}

CProjectile* CGameUnit::getTemplateProjectile()
{
    return m_pTemplateProjectile;
}

void CGameUnit::suspend()
{

}

void CGameUnit::resume()
{

}

void CGameUnit::startDoing( uint32_t dwMask)
{
    m_dwDoingFlags |= dwMask;
}

void CGameUnit::endDoing( uint32_t dwMask )
{
    m_dwDoingFlags &= (~dwMask);
}

bool CGameUnit::isDoingOr( uint32_t dwMask ) const
{
    return (m_dwDoingFlags & dwMask) != 0;
}

bool CGameUnit::isDoingAnd( uint32_t dwMask ) const
{
    return (m_dwDoingFlags & dwMask) == dwMask;
}

bool CGameUnit::isDoingNothing() const
{
    return m_dwDoingFlags == 0;
}

void CGameUnit::onTick( float fDt )
{
    CUnit::onTick(fDt);
    m_fAttackCD -= fDt;
    //CCLOG("%d %.2f/%.2f", m_iKey, m_fAttackPass, getRealAttackInterval());
    // 基础AI
    M_DEF_GM(pGm);
    if (isDoingOr(kAttacking))
    {
        attack(getLastAttackTarget(), true);
    }
    else if (isDoingNothing() || (isDoingAnd(kMoving | kWithHostility) && !isDoingOr(kAttacking)))
    {
        CGameUnit* pUnit = pGm->getUnits()->getNearestUnitInRange(getPosition(), getHostilityRange(), CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(this));
        if (pUnit)
        {
            attack(pUnit->getKey(), true);
        }
    }
}

void CGameUnit::onDie()
{
    CGameUnit* pUnit;
    CCObject* pObj;
    M_DEF_GM(pGm);
    CCArray* pArr = pGm->getUnits()->getUnitsArray();
    vector<CGameUnit*> vec;
    
    CCARRAY_FOREACH(pArr, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (!pUnit->isDead() && pUnit->getMaxLevel() && isEnemyOf(pUnit) && getDistance(pUnit) < CONST_MAX_REWARD_RANGE)
        {
            vec.push_back(pUnit);
        }
    }
    int n = vec.size();
    if (n)
    {
        int iG = getRewardGold() / n;
        int iE = getRewardExp() / n;
        CForceResouce* pRes;
        for (int i = 0; i < n; ++i)
        {
            vec[i]->addExp(iE);
            pRes = vec[i]->getForceResource();
            if (pRes)
            {
                pRes->changeGold(iG);
            }
        }
    }
    setForceResource(NULL);
    stopMove();
    stopAttack();
    setAnimation(kAnimationDie, 1, 1, kActDie, CCSequence::create(CCDelayTime::create(5), CCFadeOut::create(1), CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActDieEnd)), NULL));
    CUnit::onDie();
}

void CGameUnit::onDamaged( CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask )
{
    if (pSource && (isDoingNothing() || isDoingOr(kAutoAttack))) // (isDoingAnd(kMoving | kWithHostility) && !isDoingOr(kAttacking))
    {
        attack(pSource->getKey(), true);
    }
}

CCGameUnitSprite* CGameUnit::getSprite()
{
    return &m_oSprite;
}

float CGameUnit::getDistance( CGameUnit* pTarget )
{
    return getDistance(pTarget->getPosition());
}

float CGameUnit::getDistance( const CCPoint& roPos )
{
    M_DEF_GM(pGm);
    return pGm->getDistance(getPosition(), roPos);
}

void CGameUnit::setPosition( const CCPoint& roPos )
{
    m_oSprite.setPosition(roPos);
}

const CCPoint& CGameUnit::getPosition()
{
    return m_oSprite.getPosition();
}

void CGameUnit::turnTo( CGameUnit* pTarget )
{
    if (isFixed())
    {
        return;
    }
    m_oSprite.setFlipX(pTarget->getPosition().x < getPosition().x);
}

void CGameUnit::turnTo( bool bLeft )
{
    if (isFixed())
    {
        return;
    }
    m_oSprite.setFlipX(bLeft);
}

void CGameUnit::onActDieEnd( CCNode* pNode )
{
    //m_oSprite.removeFromParentAndCleanup(true);
    M_DEF_GM(pGm);
    pGm->moveUnitToDustbin(this);
}

CCNode* CGameUnit::getShadowNode()
{
    return m_oSprite.getShadowNode();
}

void CGameUnit::removeAllSpriteFromTheirParents()
{
    m_oSprite.getShadowNode()->removeFromParentAndCleanup(true);
    m_oSprite.removeFromParentAndCleanup(true);
}

void CGameUnit::setFixed( bool bFixed )
{
    m_bIsFixed = bFixed;
}

bool CGameUnit::isFixed() const
{
    return m_bIsFixed;
}

void CGameUnit::setForceResource( CForceResouce* pRes )
{
    CC_SAFE_RELEASE(m_pRes);
    CC_SAFE_RETAIN(pRes);
    m_pRes = pRes;
}

CForceResouce* CGameUnit::getForceResource()
{
    return m_pRes;
}

CProjectile::CProjectile()
{
}

CProjectile::~CProjectile()
{
    CC_SAFE_RELEASE(m_pAttackData);
}

bool CProjectile::init()
{
    m_pAttackData = NULL;
    setOwner(0);
    m_iTarget = 0;
    setOffsetZ(0);
    setGeneration(0);
    setProjectileType(kFollow);
    return CGameUnit::init();
}

bool CProjectile::initWithName( const char* pProjectile, const CCPoint& roAnchor /*= ccp(0.5f, 0.5f)*/ )
{
    m_pAttackData = NULL;
    setOwner(0);
    m_iTarget = 0;
    setOffsetZ(0);
    setGeneration(0);
    setProjectileType(kFollow);
    return CGameUnit::initWithName(pProjectile, roAnchor);
}


CAttackData* CProjectile::getAttackData() const
{
    return m_pAttackData;
}

void CProjectile::setAttackData( CAttackData* pAttackData )
{
    if (pAttackData != m_pAttackData)
    {
        CC_SAFE_RELEASE(m_pAttackData);
    }

    m_pAttackData = pAttackData;
    CC_SAFE_RETAIN(m_pAttackData);
}

void CProjectile::onActMoveEnd( CCNode* pNode )
{
    CGameUnit::onActMoveEnd(pNode);
    //setAnimation(kAnimationDie, 1, 0.1, kActDie, NULL);
    onDie();
}

void CProjectile::onDie()
{
    M_DEF_GM(pGm);
    CCAnimation* pAni;
    CCAction* pAct;
    CGameUnit* pTarget = pGm->getUnitByKey(getTarget());
    CGameUnit* pOwner = pGm->getUnitByKey(getOwner());

    if (pTarget && pOwner)
    {
        pTarget->damagedAdv(getAttackData(), pOwner);
    }
    
    //stopMove();
    switch (m_eProjectileType)
    {
    case kFollow:
        setAnimation(kAnimationDie, 1, 1, kActDie, CCCallFuncN::create(this, callfuncN_selector(CProjectile::onActDieEnd)));
        break;

    case kLightning:

        pAni = pGm->getUnitAnimation(getName(), m_astAniInfo[kAnimationDie].sAnimation.c_str());
        pAni->setDelayPerUnit(m_astAniInfo[kAnimationDie].fDelay);
        pAct = CCSequence::createWithTwoActions(CCLightning::create(pAni, pOwner->getSprite(), pTarget->getSprite(), pOwner->getProjectileBirthOffsetX(), pOwner->getProjectileBirthOffsetY(), pTarget->getHalfOfHeight()), CCCallFuncN::create(this, callfuncN_selector(CProjectile::onActDieEnd)));
        pAct->setTag(kActDie);
        m_oSprite.runAction(pAct);
        break;
    }

    CUnit::onDie(); // Don't call CGameUnit::onDie()
}

void CProjectile::onActDieEnd( CCNode* pNode )
{
    //m_oSprite.removeFromParentAndCleanup(true);
    M_DEF_GM(pGm);
    pGm->moveProjectileToDustbin(this);
}

CCObject* CProjectile::copyWithZone( CCZone* pZone )
{
    CProjectile* pProj = CProjectile::createWithName(getName());
    for (int i = 0; i < CONST_MAX_ANIMATION; ++i)
    {
        ARR_ANIMATION_INFO& roAni = m_astAniInfo[i];
        pProj->prepareAnimation((ANIMATION_INDEX)i, roAni.sAnimation.c_str(), roAni.fDelay);
    }
    pProj->setProjectileType(getProjectileType());
    return pProj;
}

void CProjectile::setTarget( int iTarget )
{
    M_DEF_GM(pGm);
    setTarget(pGm->getUnitByKey(iTarget));
}

void CProjectile::setTarget( CGameUnit* pTarget )
{
    m_iTarget = pTarget->getKey();
    setOffsetZ(pTarget->getHalfOfHeight());
}

int CProjectile::getTarget() const
{
    return m_iTarget;
}

CCUnitLayer::CCUnitLayer()
{
}

bool CCUnitLayer::init()
{
    m_fUnitTickInterval = 0;
    return CCLayerColor::init();
}

bool CCUnitLayer::initWithColor( const ccColor4B& color )
{
    m_fUnitTickInterval = 0;
    return CCLayerColor::initWithColor(color);
}

void CCUnitLayer::onEnter()
{
    CCLayerColor::onEnter();
    setUnitTickInterval(0.1);
}

void CCUnitLayer::onExit()
{
    unschedule(schedule_selector(CCUnitLayer::onTickEvent));
    CCLayerColor::onExit();
}

void CCUnitLayer::onTickEvent( float fDt )
{
    M_DEF_GM(pGm);
    CCArray* pUnitDustbin = pGm->getUnitDustbin();
    CCArray* pProjectileDustbin = pGm->getProjectileDustbin();
    CCArray* pUnits = pGm->getUnits()->getUnitsArray();
    CCArray* pProjectiles = pGm->getProjectiles()->getUnitsArray();

    CGameUnit* pUnit;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->onTick(fDt);
    }
    CCARRAY_FOREACH(pUnitDustbin, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->removeAllSpriteFromTheirParents();
        pUnits->removeObject(pUnit);
    }
    pUnitDustbin->removeAllObjects();

    CProjectile* pProjectile;
    CCARRAY_FOREACH(pProjectiles, pObj)
    {
        pProjectile = dynamic_cast<CProjectile*>(pObj);
        pProjectile->onTick(fDt);
    }
    CCARRAY_FOREACH(pProjectileDustbin, pObj)
    {
        pProjectile = dynamic_cast<CProjectile*>(pObj);
        pProjectile->removeAllSpriteFromTheirParents();
        pProjectiles->removeObject(pProjectile);
    }
    pProjectileDustbin->removeAllObjects();
}

void CCUnitLayer::addUnit( CGameUnit* pUnit )
{
    M_DEF_GM(pGm);
    pGm->addUnitToEventDriver(pUnit);
    addChild(pUnit->getSprite());
    addChild(pUnit->getShadowNode(), 10);
}

void CCUnitLayer::addProjectile( CProjectile* pProjectile )
{
    M_DEF_GM(pGm);
    pGm->addProjectileToEventDriver(pProjectile);
    addChild(pProjectile->getSprite());
}

void CCUnitLayer::setUnitTickInterval( float fInterval )
{
    m_fUnitTickInterval = fInterval;
    schedule(schedule_selector(CCUnitLayer::onTickEvent), m_fUnitTickInterval);
}

float CCUnitLayer::getUnitTickInterval() const
{
    return m_fUnitTickInterval;
}

CCWinUnitLayer::CCWinUnitLayer()
{

}

bool CCWinUnitLayer::init()
{
    setBufferEffectParam(0, 0, 0);
    m_bIsTouching = false;
    return CCUnitLayer::init();
}

bool CCWinUnitLayer::initWithColor( const ccColor4B& color )
{
    setBufferEffectParam(0, 0, 0);
    m_bIsTouching = false;
    return CCUnitLayer::initWithColor(color);
}

void CCWinUnitLayer::setBackGroundSprite( CCSprite* pSprite )
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));

}
  
void CCWinUnitLayer::setBackGroundSprite( CCSprite* pSprite, int zOrder, int tag )
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite, zOrder, tag);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
}

void CCWinUnitLayer::onEnter()
{
    CCUnitLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    schedule(schedule_selector(CCWinUnitLayer::bufferWindowEffect), 1.0 / 60);
}

void CCWinUnitLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    unschedule(schedule_selector(CCWinUnitLayer::bufferWindowEffect));
    CCUnitLayer::onExit();
}

bool CCWinUnitLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    m_bIsTouching = true;
    m_fTouchMovedDuration = 0;
    m_oMoveStart = pTouch->getLocation();
    m_fMoveDelta = 0;
    m_bCanMove = false;
    return true;
}

void CCWinUnitLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint oT = ccpAdd(getPosition(), pTouch->getDelta());
    adjustWinPos(oT);
    setPosition(oT);
}

void CCWinUnitLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    m_bIsTouching = false;
    m_fMoveDelta = ccpDistance(pTouch->getLocation(), m_oMoveStart);
    m_fMoveR = -ccpToAngle(ccpSub(pTouch->getLocation(), m_oMoveStart));
    (m_fTouchMovedDuration <= CONST_MAX_CAN_MOVE_DURATION && !isClickAction()) && (m_bCanMove = true);
}

void CCWinUnitLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
}

void CCWinUnitLayer::touchDelegateRetain()
{
    this->retain();
}

void CCWinUnitLayer::touchDelegateRelease()
{
    this->release();
}

void CCWinUnitLayer::bufferWindowEffect(float fDt)
{
    if (m_bIsTouching)
    {
        m_fTouchMovedDuration += fDt;
        return;
    }
    static CCSize oSzWin = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize();
    CCPoint oT = getPosition();

    bool bOut = false;
    float fMax;
    if (oT.x > -m_fBuffRange * getScaleX())
    {
        oT.x += (-oT.x - m_fBuffRange) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    else if (oT.x < (fMax = (oSzWin.width - oSz.width + m_fBuffRange) * getScaleX()))
    {
        oT.x += (-oT.x + fMax) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    if (oT.y > -m_fBuffRange * getScaleY())
    {
        oT.y += (-oT.y - m_fBuffRange) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    else if (oT.y < (fMax = (oSzWin.height - oSz.height + m_fBuffRange) * getScaleY()))
    {
        oT.y += (-oT.y + fMax) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    if (bOut)
    {
        setPosition(oT);
        m_fMoveDelta = 0;
    }
    else if (m_bCanMove)
    {
        m_fMoveDelta *= m_fMoveK;
        if (m_fMoveDelta >= CONST_MIN_MOVE_DELTA)
        {
            float fMove = m_fMoveDelta * fDt / m_fTouchMovedDuration;
            CCPoint oP = ccpAdd(oT, ccp(cos(m_fMoveR) * fMove, sin(-m_fMoveR) * fMove));
            adjustWinPos(oP);
            setPosition(oP);
        }
    }
}

void CCWinUnitLayer::setBufferEffectParam( float fMoveK, float fBuffRange, float fEdgeK )
{
    m_fMoveK = MIN(1, MAX(0, fMoveK));

    CCSize oSz = getContentSize();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    oSz.width = MAX(0, oSz.width - oWinSz.width);
    oSz.height = MAX(0, oSz.height - oWinSz.height);
    m_fBuffRange = MIN(MIN(oSz.width, oSz.height) / 2, MAX(0, fBuffRange));

    m_fEdgeK = MIN(1, MAX(0, fEdgeK));
}

float CCWinUnitLayer::getTouchMovedDuration() const
{
    return m_fTouchMovedDuration;
}

float CCWinUnitLayer::getTouchMovedDistance() const
{
    return m_fMoveDelta;
}

float CCWinUnitLayer::getTouchMovedRadian() const
{
    return m_fMoveR;
}

bool CCWinUnitLayer::isSlideAction() const
{
    return m_bCanMove;
}

bool CCWinUnitLayer::isClickAction() const
{
    return m_fMoveDelta < CONST_MIN_MOVE_DELTA;
}

void CCWinUnitLayer::adjustWinPos( CCPoint& roPos )
{
    static CCSize oSzWin = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize();
    roPos.x = MAX(roPos.x, (oSzWin.width - oSz.width) * getScaleX());
    roPos.y = MAX(roPos.y, (oSzWin.height - oSz.height) * getScaleY());
    roPos.x = MIN(roPos.x, 0);
    roPos.y = MIN(roPos.y, 0);
}

const float CCWinUnitLayer::CONST_MIN_MOVE_DELTA = 10.0;
const float CCWinUnitLayer::CONST_MAX_CAN_MOVE_DURATION = 0.15;

CUnitGroup::CUnitGroup()
{
}

bool CUnitGroup::init()
{
    m_oArrUnits.init();
    return true;
}

bool CUnitGroup::initWithUnitsInRange( CUnitGroup* pSource, const CCPoint& roPos, float fRadius, int iMaxCount /*= INFINITE*/, CONDITIONFUNC pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    m_oArrUnits.init();
    int i = 0;
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(pSource->getUnitsArray(), pObj)
    {
        if (i == iMaxCount)
        {
            return true;
        }
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (pUnit->getDistance(roPos) < fRadius && pBoolFunc && pBoolFunc(pUnit, pParam))
        {
            m_oArrUnits.addObject(pObj);
            ++i;
        }
        
    }
    return true;
}

bool CUnitGroup::initWithCondition( CUnitGroup* pSource, int iMaxCount /*= INFINITE*/, CONDITIONFUNC pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    m_oArrUnits.init();
    int i = 0;
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(pSource->getUnitsArray(), pObj)
    {
        if (i == iMaxCount)
        {
            return true;
        }
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (pBoolFunc && pBoolFunc(pUnit, pParam))
        {
            m_oArrUnits.addObject(pObj);
            ++i;
        }

    }
    return true;
}

CGameUnit* CUnitGroup::getUnitByIndex( int iIndex )
{
    return dynamic_cast<CGameUnit*>(m_oArrUnits.objectAtIndex(iIndex));
}

CGameUnit* CUnitGroup::getUnitByKey( int iKey )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (pUnit->getKey() == iKey)
        {
            return pUnit;
        }
    }
    return NULL;
}

CGameUnit* CUnitGroup::getRandomUnit()
{
    return dynamic_cast<CGameUnit*>(m_oArrUnits.randomObject());
}

CGameUnit* CUnitGroup::getNearestUnitInRange( const CCPoint& roPos, float fRadius, CONDITIONFUNC pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CGameUnit* pTarget = NULL;
    float fMinDis = FLT_MAX;
    float fDis;

    CCArray* pUnits = getUnitsArray();
    CGameUnit* pUnit = NULL;
    CCObject* pObj;
    CCARRAY_FOREACH(pUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if ((fDis = ccpDistance(pUnit->getPosition(), roPos)) < fRadius && fMinDis > fDis && pBoolFunc && pBoolFunc(pUnit, pParam))
        {
            pTarget = pUnit;
            fMinDis = fDis;
        }
    }
    return pTarget;
}

void CUnitGroup::addUnit( CGameUnit* pUnit )
{
    m_oArrUnits.addObject(pUnit);
}

void CUnitGroup::delUnit( CGameUnit* pUnit )
{
    m_oArrUnits.removeObject(pUnit);
}

CCArray* CUnitGroup::getUnitsArray()
{
    return &m_oArrUnits;
}

CUnitGroup* CUnitGroup::getUnitsInRange( const CCPoint& roPos, float fRadius, int iMaxCount /*= INFINITE*/, CONDITIONFUNC pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    return CUnitGroup::createWithUnitsInRange(this, roPos, fRadius, iMaxCount, pBoolFunc, pParam);
}

CUnitGroup* CUnitGroup::getUnits( int iMaxCount /*= INFINITE*/, CONDITIONFUNC pBoolFunc, void* pParam /*= NULL*/ )
{
    return CUnitGroup::createWithCondition(this, iMaxCount, pBoolFunc, pParam);
}

void CUnitGroup::cleanUnits()
{
    m_oArrUnits.removeAllObjects();
}

void CUnitGroup::addUnits( CUnitGroup* pUnits )
{
    m_oArrUnits.addObjectsFromArray(pUnits->getUnitsArray());
}

void CUnitGroup::addUnits( CCArray* pUnits )
{
    m_oArrUnits.addObjectsFromArray(pUnits);
}

void CUnitGroup::delUnits( CUnitGroup* pUnits )
{
    m_oArrUnits.removeObjectsInArray(pUnits->getUnitsArray());
}

void CUnitGroup::delUnits( CCArray* pUnits )
{
    m_oArrUnits.removeObjectsInArray(pUnits);
}

int CUnitGroup::getUnitsCount()
{
    return m_oArrUnits.count();
}

void CUnitGroup::setRangePosition( const CCPoint& roPos, float fRadius )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->setPosition(ccp(roPos.x - fRadius + rand() % (int)fRadius, roPos.y - fRadius + rand() % (int)fRadius));
    }
}

void CUnitGroup::turnTo( bool bLeft )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->turnTo(bLeft);
    }
}

void CUnitGroup::moveTo( const CCPoint& roPos, bool bWithHostility /*= false*/, bool bCancelAttack /*= true*/, bool bAutoFlipX /*= true*/ )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->moveTo(roPos, bWithHostility, bCancelAttack, bAutoFlipX);
    }
}

void CUnitGroup::followTo( int iTargetKey, bool bWithHostility /*= false*/, bool bCancelAttack /*= true*/, bool bAutoFlipX /*= true*/, float fMaxOffsetY /*= 0.0*/ )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->followTo(iTargetKey, bWithHostility, bCancelAttack, bAutoFlipX, fMaxOffsetY);
    }
}

void CUnitGroup::stopMove()
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->stopMove();
    }
}

void CUnitGroup::attack( int iTargetKey )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->attack(iTargetKey);
    }
}

void CUnitGroup::stopAttack()
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->stopAttack();
    }
}

void CUnitGroup::moveAlongPath( CUnitPath* pPath, bool bRestart /*= false*/, bool bWithHostility /*= true*/, float fBuffArrive /*= FLT_EPSILON*/ )
{
    CPathGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CPathGameUnit*>(pObj);
        if (pUnit)
        {
            pUnit->moveAlongPath(pPath, bRestart, bWithHostility, fBuffArrive);
        }
    }
}

void CUnitGroup::damagedAdv( CAttackData* pAttack, CUnit* pSource )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedAdv(pAttack, pSource);
    }
}

void CUnitGroup::damagedMid( CAttackData* pAttack, CUnit* pSource )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedMid(pAttack, pSource);
    }
}

void CUnitGroup::damagedBot( float fDamage, CUnit* pSource )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedBot(fDamage, pSource);
    }
}

void CUnitGroup::addSkill( CSkill* pSkill )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->addSkill(dynamic_cast<CSkill*>(pSkill->copy()));
    }
}

void CUnitGroup::addPackage( CUnitPackage* pPackage )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->addPackage(pPackage);
    }
}

void CUnitGroup::addBuff( CBuffSkill* pBuff, bool bForce /*= false*/ )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->addBuff(dynamic_cast<CBuffSkill*>(pBuff->copy()), bForce);
    }
}

CGameUnit* CUnitGroup::copyUnit( int iKey )
{
    return dynamic_cast<CGameUnit*>(getUnitByKey(iKey)->copy());
}

bool CUnitGroup::isLivingAllyOf( CGameUnit* pUnit, CUnitForce* pParam)
{
    return !pUnit->isDead() && pUnit->isAllyOf(pParam);
}

bool CUnitGroup::isLivingEnemyOf( CGameUnit* pUnit, CUnitForce* pParam)
{
    return !pUnit->isDead() && pUnit->isEnemyOf(pParam);
}

CUnitInfo::CUnitInfo( const char* pName, const CCPoint& roAnchor, float fHalfOfWidth, float fHalfOfHeight, float fScale, float fActMoveDelay, float fActDieDelay, float fAct1Delay, float fAct2Delay, float fAct3Delay, float fAct4Delay, float fAct5Delay, float fAct6Delay, const ARR_ATTACK_ANI& roArrAttackAnis, float fBaseMoveSpeed, float fBaseAttackInterval, float fAttackEffectDelay, float fAttackMinRange, float fAttackRange, float fHostilityRange, CGameUnit::WEAPON_TYPE eWeaponType, int iProjectileKey, float fProjectileMoveSpeed, float fProjectileScale, float fProjectileMaxOffsetY, float fProjectileBirthOffsetX, float fProjectileBirthOffsetY, const CAttackValue& roBaseAttackValue, float fExAttackRandomRange, CArmorValue::ARMOR_TYPE eArmorType, float fBaseArmorValue, int iForceIndex, uint32_t dwForceAlly, float fMaxHp, bool bIsFixed, int iRewardGold, int iRewardExp )
    : m_sName(pName)
    , m_oAnchor(roAnchor)
    , m_fHalfOfWidth(fHalfOfWidth)
    , m_fHalfOfHeight(fHalfOfHeight)
    , m_fScale(fScale)
    , m_fActMoveDelay(fActMoveDelay)
    , m_fActDieDelay(fActDieDelay)
    , m_fAct1Delay(fAct1Delay)
    , m_fAct2Delay(fAct2Delay)
    , m_fAct3Delay(fAct3Delay)
    , m_fAct4Delay(fAct4Delay)
    , m_fAct5Delay(fAct5Delay)
    , m_fAct6Delay(fAct6Delay)
    , m_fBaseMoveSpeed(fBaseMoveSpeed)
    , m_fBaseAttackInterval(fBaseAttackInterval)
    , m_fAttackEffectDelay(fAttackEffectDelay)
    , m_fAttackMinRange(fAttackMinRange)
    , m_fAttackRange(fAttackRange)
    , m_fHostilityRange(fHostilityRange)
    , m_eWeaponType(eWeaponType)
    , m_iProjectileKey(iProjectileKey)
    , m_fProjectileMoveSpeed(fProjectileMoveSpeed)
    , m_fProjectileScale(fProjectileScale)
    , m_fProjectileMaxOffsetY(fProjectileMaxOffsetY)
    , m_fProjectileBirthOffsetX(fProjectileBirthOffsetX)
    , m_fProjectileBirthOffsetY(fProjectileBirthOffsetY)
    , m_oBaseAttackValue(roBaseAttackValue)
    , m_fExAttackRandomRange(fExAttackRandomRange)
    , m_eArmorType(eArmorType)
    , m_fBaseArmorValue(fBaseArmorValue)
    , m_iForceIndex(iForceIndex)
    , m_dwForceAlly(dwForceAlly)
    , m_fMaxHp(fMaxHp)
    , m_bIsFixed(bIsFixed)
    , m_iRewardGold(iRewardGold)
    , m_iRewardExp(iRewardExp)
{
    int n = roArrAttackAnis.count();
    const CGameUnit::ANIMATION_INDEX* pAnis = roArrAttackAnis;
    for (int i = 0; i < n; ++i)
    {
        m_vecAttackAni.push_back(pAnis[i]);
    }
}

CUnitInfo::~CUnitInfo(void)
{
}

const uint16_t CUnitInfoPatch::CONST_FILE_DATA_SIZE
    = sizeof(m_iOrgUnitIndex)
    + sizeof(m_szName)
    + sizeof(m_fBaseMoveSpeed)
    + sizeof(m_fBaseAttackInterval)
    + sizeof(m_fAttackMinRange)
    + sizeof(m_fAttackRange)
    + sizeof(m_fHostilityRange)
    + sizeof(m_fProjectileMoveSpeed)
    + sizeof(m_fProjectileScale)
    + sizeof(m_oBaseAttackValue.m_afAttack)
    + sizeof(m_fExAttackRandomRange)
    + sizeof(m_eArmorType)
    + sizeof(m_fBaseArmorValue)
    + sizeof(m_fMaxHp)
    + sizeof(m_bIsFixed)
    + sizeof(m_iRewardGold)
    + sizeof(m_iRewardExp);

CUnitInfoPatch::CUnitInfoPatch()
    : m_iOrgUnitIndex(0)
    , m_fBaseMoveSpeed(0)
    , m_fBaseAttackInterval(0)
    , m_fAttackMinRange(0)
    , m_fAttackRange(0)
    , m_fHostilityRange(0)
    , m_fProjectileMoveSpeed(0)
    , m_fProjectileScale(0)
    , m_fExAttackRandomRange(0)
    , m_eArmorType(CArmorValue::ARMOR_TYPE(0))
    , m_fBaseArmorValue(0)
    , m_fMaxHp(0)
    , m_bIsFixed(false)
    , m_iRewardGold(0)
    , m_iRewardExp(0)
{
    memset(m_szName, 0, sizeof(m_szName));
}

bool CUnitInfoPatch::initWithFileStream( CGameFile* pFile )
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
            || pFile->read(&m_iOrgUnitIndex) != 1
            || pFile->read(m_szName, sizeof(m_szName)) != sizeof(m_szName)
            || pFile->read(&m_fBaseMoveSpeed) != 1
            || pFile->read(&m_fBaseAttackInterval) != 1
            || pFile->read(&m_fAttackMinRange) != 1
            || pFile->read(&m_fAttackRange) != 1
            || pFile->read(&m_fHostilityRange) != 1
            || pFile->read(&m_fProjectileMoveSpeed) != 1
            || pFile->read(&m_fProjectileScale) != 1
            || pFile->read(&m_oBaseAttackValue.m_afAttack) != 1
            || pFile->read(&m_fExAttackRandomRange) != 1
            || pFile->read(&m_eArmorType) != 1
            || pFile->read(&m_fBaseArmorValue) != 1
            || pFile->read(&m_fMaxHp) != 1
            || pFile->read(&m_bIsFixed) != 1
            || pFile->read(&m_iRewardGold) != 1
            || pFile->read(&m_iRewardExp) != 1
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

bool CUnitInfoPatch::initWithUnitInfo( int iUnitInfoIndex )
{
    M_DEF_UM(pUm);
    CUnitInfo* pUi = pUm->unitInfoByIndex(iUnitInfoIndex);
    if (!pUi)
    {
        return false;
    }

    m_iOrgUnitIndex = iUnitInfoIndex;
    strcpy(m_szName, pUi->m_sName.c_str());
    m_fBaseMoveSpeed = pUi->m_fBaseMoveSpeed;
    m_fBaseAttackInterval = pUi->m_fBaseAttackInterval;
    m_fAttackMinRange = pUi->m_fAttackMinRange;
    m_fAttackRange = pUi->m_fAttackRange;
    m_fHostilityRange = pUi->m_fHostilityRange;
    m_fProjectileMoveSpeed = pUi->m_fProjectileMoveSpeed;
    m_fProjectileScale = pUi->m_fProjectileScale;
    m_oBaseAttackValue = pUi->m_oBaseAttackValue;
    m_fExAttackRandomRange = pUi->m_fExAttackRandomRange;
    m_eArmorType = pUi->m_eArmorType;
    m_fBaseArmorValue = pUi->m_fBaseArmorValue;
    m_fMaxHp = pUi->m_fMaxHp;
    m_bIsFixed = pUi->m_bIsFixed;
    m_iRewardGold = pUi->m_iRewardGold;
    m_iRewardExp = pUi->m_iRewardExp;
    m_vecSkills.clear();
    return true;
}

void CUnitInfoPatch::writeToFileStream( FILE* pFile )
{
    fwrite(&CONST_FILE_DATA_SIZE, sizeof(CONST_FILE_DATA_SIZE), 1, pFile);
    fwrite(&m_iOrgUnitIndex, sizeof(m_iOrgUnitIndex), 1, pFile);
    fwrite(m_szName, sizeof(char), sizeof(m_szName), pFile);
    fwrite(&m_fBaseMoveSpeed, sizeof(m_fBaseMoveSpeed), 1, pFile);
    fwrite(&m_fBaseAttackInterval, sizeof(m_fBaseAttackInterval), 1, pFile);
    fwrite(&m_fAttackMinRange, sizeof(m_fAttackMinRange), 1, pFile);
    fwrite(&m_fAttackRange, sizeof(m_fAttackRange), 1, pFile);
    fwrite(&m_fHostilityRange, sizeof(m_fHostilityRange), 1, pFile);
    fwrite(&m_fProjectileMoveSpeed, sizeof(m_fProjectileMoveSpeed), 1, pFile);
    fwrite(&m_fProjectileScale, sizeof(m_fProjectileScale), 1, pFile);
    fwrite(&m_oBaseAttackValue.m_afAttack, sizeof(m_oBaseAttackValue.m_afAttack), 1, pFile);
    fwrite(&m_fExAttackRandomRange, sizeof(m_fExAttackRandomRange), 1, pFile);
    fwrite(&m_eArmorType, sizeof(m_eArmorType), 1, pFile);
    fwrite(&m_fBaseArmorValue, sizeof(m_fBaseArmorValue), 1, pFile);
    fwrite(&m_fMaxHp, sizeof(m_fMaxHp), 1, pFile);
    fwrite(&m_bIsFixed, sizeof(m_bIsFixed), 1, pFile);
    fwrite(&m_iRewardGold, sizeof(m_iRewardGold), 1, pFile);
    fwrite(&m_iRewardExp, sizeof(m_iRewardExp), 1, pFile);
    uint16_t wSkillCount = m_vecSkills.size();
    fwrite(&wSkillCount, sizeof(wSkillCount), 1, pFile);
    for (uint16_t i = 0; i < wSkillCount; ++i)
    {
        fwrite(&m_vecSkills[i], sizeof(m_vecSkills[i]), 1, pFile);
    }
}

bool CUnitInfoPatchManager::init()
{
    return true;
}

bool CUnitInfoPatchManager::initWithFile( const char* pFileName )
{
    m_vecPatches.clear();
    return addPatches(pFileName) > 0;
}

int CUnitInfoPatchManager::addPatch( const CUnitInfoPatch& roPatch )
{
    m_vecPatches.push_back(roPatch);
    return m_vecPatches.size() - 1;
}

int CUnitInfoPatchManager::addNewPatch( int iUnitIndex )
{
    CUnitInfoPatch oUip;
    oUip.initWithUnitInfo(iUnitIndex);
    return addPatch(oUip);
}

int CUnitInfoPatchManager::addPatches( const char* pFileName )
{
    CGameFile* pFile = CGameFile::create(pFileName, "rb");
    uint32_t dwHdr = 0;
    int iRet = 0;
    do 
    {
        pFile->read(&dwHdr);
        if (dwHdr != 'PIU')
        {
            break;
        }
        CUnitInfoPatch oPatch;
        while (oPatch.initWithFileStream(pFile))
        {
            ++iRet;
            addPatch(oPatch);
        }
    } while (false);
    
    return iRet;
}

void CUnitInfoPatchManager::saveAsFile( const char* pFileName )
{
    string sFullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
    FILE* pFile = fopen(sFullName.c_str(), "wb");
    fwrite("UIP", 1, 4, pFile);
    for (VEC_PATCHES::iterator it = m_vecPatches.begin(); it != m_vecPatches.end(); ++it)
    {
        it->writeToFileStream(pFile);
    }
    fclose(pFile);
}

void CUnitInfoPatchManager::setPatch(int iIndex, const CUnitInfoPatch &roPatch)
{
    m_vecPatches[iIndex] = roPatch;
}

int CUnitInfoPatchManager::count() const
{
    return m_vecPatches.size();
}

CUnitInfoPatch* CUnitInfoPatchManager::patchByIndex( int iIndex )
{
    if (iIndex >= 0 && iIndex < (int)m_vecPatches.size())
    {
        return &m_vecPatches[iIndex];
    }
    return NULL;
}

CGameUnit* CUnitInfoPatchManager::unitByIndex( int iIndex )
{
    M_DEF_UM(pUm);
    CUnitInfoPatch* pPatch = patchByIndex(iIndex);
    if (!pPatch)
    {
        return NULL;
    }
    CGameUnit* pU = pUm->unitByInfo(pPatch->m_iOrgUnitIndex);
    if (!pU)
    {
        return NULL;
    }
    patch(pU, pPatch);
    return pU;
}

CPathGameUnit* CUnitInfoPatchManager::pathUnitByIndex( int iIndex )
{
    M_DEF_UM(pUm);
    CUnitInfoPatch* pPatch = patchByIndex(iIndex);
    if (!pPatch)
    {
        return NULL;
    }
    CPathGameUnit* pU = pUm->pathUnitByInfo(pPatch->m_iOrgUnitIndex);
    if (!pU)
    {
        return NULL;
    }
    patch(pU, pPatch);
    return pU;
}

CHeroUnit* CUnitInfoPatchManager::heroByIndex( int iIndex )
{
    M_DEF_UM(pUm);
    CUnitInfoPatch* pPatch = patchByIndex(iIndex);
    if (!pPatch)
    {
        return NULL;
    }
    CHeroUnit* pU = pUm->heroByInfo(pPatch->m_iOrgUnitIndex);
    if (!pU)
    {
        return NULL;
    }
    patch(pU, pPatch);
    return pU;
}

void CUnitInfoPatchManager::patch( CGameUnit* pUnit, CUnitInfoPatch* pPatch )
{
    M_DEF_OS(pOs);
    pUnit->setBaseMoveSpeed(pPatch->m_fBaseMoveSpeed);
    pUnit->setBaseAttackInterval(pPatch->m_fBaseAttackInterval);
    pUnit->setAttackMinRange(pPatch->m_fAttackMinRange);
    pUnit->setAttackRange(pPatch->m_fAttackRange);
    pUnit->setHostilityRange(pPatch->m_fHostilityRange);
    pUnit->setProjectileMoveSpeed(pPatch->m_fProjectileMoveSpeed);
    pUnit->setProjectileScale(pPatch->m_fProjectileScale);
    pUnit->setBaseAttackValue(pPatch->m_oBaseAttackValue);
    pUnit->setArmorType(pPatch->m_eArmorType);
    pUnit->setBaseArmorValue(pPatch->m_fBaseArmorValue);
    pUnit->setMaxHp(pPatch->m_fMaxHp);
    pUnit->setFixed(pPatch->m_bIsFixed);
    pUnit->setRewardGold(pPatch->m_iRewardGold);
    pUnit->setRewardExp(pPatch->m_iRewardExp);
    for (CUnitInfoPatch::VEC_SKILL_INDEX::iterator it = pPatch->m_vecSkills.begin(); it != pPatch->m_vecSkills.end(); ++it)
    {
        pUnit->addSkill(pOs->skill(*it));
    }
}

CUnitInfoPatchManager* CUnitInfoPatchManager::sharedUnitInfoPatchManager()
{
    static CUnitInfoPatchManager* pUipm = NULL;
    if (pUipm)
    {
        return pUipm;
    }
    pUipm = CUnitInfoPatchManager::create();
    CC_SAFE_RETAIN(pUipm);
    return pUipm;
}

CUnitManager* CUnitManager::m_pInst = NULL;

CUnitManager::CUnitManager()
{
}

CUnitManager::~CUnitManager()
{
}

bool CUnitManager::init()
{
    return true;
}

CUnitManager* CUnitManager::sharedUnitManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CUnitManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CUnitManager::addUnitInfo( int iUnitInfoIndex, const CUnitInfo& roUnitInfo )
{
    m_mapUnitInfo.insert(pair<int, CUnitInfo>(iUnitInfoIndex, roUnitInfo));
}

CGameUnit* CUnitManager::unitByInfo( int iUnitInfoIndex )
{
    M_DEF_OU(pOu);
    UNIT_MAP::iterator it = m_mapUnitInfo.find(iUnitInfoIndex);
    if (it == m_mapUnitInfo.end())
    {
        return NULL;
    }
    CGameUnit* pUnit = CGameUnit::createWithInfo(it->second);
    return pUnit;
}

CPathGameUnit* CUnitManager::pathUnitByInfo( int iUnitInfoIndex )
{
    M_DEF_OU(pOu);
    UNIT_MAP::iterator it = m_mapUnitInfo.find(iUnitInfoIndex);
    if (it == m_mapUnitInfo.end())
    {
        return NULL;
    }
    CPathGameUnit* pUnit = CPathGameUnit::createWithInfo(it->second);
    return pUnit;
}

CHeroUnit* CUnitManager::heroByInfo( int iUnitInfoIndex )
{
    M_DEF_OU(pOu);
    UNIT_MAP::iterator it = m_mapUnitInfo.find(iUnitInfoIndex);
    if (it == m_mapUnitInfo.end())
    {
        return NULL;
    }
    CHeroUnit* pUnit = CHeroUnit::createWithInfo(it->second);
    return pUnit;
}

CUnitInfo* CUnitManager::unitInfoByIndex( int iUnitIndex )
{
    M_DEF_OU(pOu);
    UNIT_MAP::iterator it = m_mapUnitInfo.find(iUnitIndex);
    if (it != m_mapUnitInfo.end())
    {
        return &it->second;
    }
    return NULL;
}

CProjectileManager* CProjectileManager::m_pInst = NULL;

CProjectileManager::CProjectileManager()
{
}

CProjectileManager::~CProjectileManager()
{
}

bool CProjectileManager::init()
{
    m_oArrProjectile.init();
    return true;
}

CProjectileManager* CProjectileManager::sharedProjectileManager()
{
    if (!m_pInst)
    {
        m_pInst = CProjectileManager::create();
        CC_SAFE_RETAIN(m_pInst);
    }
    return m_pInst;
}

void CProjectileManager::cleanUp()
{
    m_oArrProjectile.cleanUnits();
}

void CProjectileManager::addProjectile( CProjectile* pProjectile )
{
    m_oArrProjectile.addUnit(pProjectile);
}

CProjectile* CProjectileManager::getProjectile( int iKey )
{
    return dynamic_cast<CProjectile*>(m_oArrProjectile.getUnitByKey(iKey));
}

CProjectile* CProjectileManager::copyProjectile( int iKey )
{
    return dynamic_cast<CProjectile*>(m_oArrProjectile.getUnitByKey(iKey)->copy());
}

CPathGameUnit::~CPathGameUnit()
{
    CC_SAFE_RELEASE(m_pCurPath);
}

bool CUnitPath::init()
{
    m_vecPoints.clear();
    return true;
}

bool CUnitPath::initWithPoints( const VEC_POINTS& roVecPoints )
{
    m_vecPoints = roVecPoints;
    return true;
}

void CUnitPath::addPoint( const CCPoint& roPos )
{
    m_vecPoints.push_back(roPos);
}

const CCPoint* CUnitPath::getCurTargetPoint(uint32_t dwCurPos)
{
    if (dwCurPos < m_vecPoints.size())
    {
        return &m_vecPoints[dwCurPos];
    }
    return NULL;
}

void CUnitPath::arriveCurTargetPoint(uint32_t& rCurPos)
{
    if (rCurPos < m_vecPoints.size())
    {
        ++rCurPos;
    }
}

void CUnitPath::addPoints( const char* pFileName )
{
    M_DEF_FU(pFu);
    CGameFile* pFile = CGameFile::create(pFileName, "rb");
    if (!pFile)
    {
        return;
    }
    uint32_t dwHdr = 0;
    pFile->read(&dwHdr);
    if (dwHdr != 'HTP')
    {
        return;
    }
    CCPoint oP;
    for (;;)
    {
        if (pFile->read(&oP.x) != 1 || pFile->read(&oP.y) != 1)
        {
            break;
        }
        addPoint(oP);
    }
}

void CUnitPath::saveAsFile( const char* pFileName )
{
    string sFullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
    FILE* pFile = fopen(sFullName.c_str(), "wb");
    fwrite("PTH", 1, 4, pFile);
    for (CUnitPath::VEC_POINTS::iterator it = m_vecPoints.begin(); it != m_vecPoints.end(); ++it)
    {
        fwrite(&it->x, sizeof(it->x), 1, pFile);
        fwrite(&it->y, sizeof(it->y), 1, pFile);
    }
    fclose(pFile);
}

bool CUnitPath::initWithFile( const char* pFileName )
{
    CUnitPath::init();
    addPoints(pFileName);
    return true;
}

bool CPathGameUnit::initWithName( const char* pUnit, const CCPoint& roAnchor /*= ccp(0.5f, 0.5f)*/ )
{
    m_pCurPath = NULL;
    m_dwCurPos = 0;
    m_bWithHostility = false;
    m_fBuffArrive = FLT_EPSILON;
    return CGameUnit::initWithName(pUnit, roAnchor);
}

bool CPathGameUnit::initWithInfo( const CUnitInfo& roUnitInfo )
{
    m_pCurPath = NULL;
    m_dwCurPos = 0;
    m_bWithHostility = false;
    m_fBuffArrive = FLT_EPSILON;
    return CGameUnit::initWithInfo(roUnitInfo);
}

void CPathGameUnit::onTick( float fDt )
{
    const CCPoint* pTarget;
    if (m_pCurPath && (pTarget = m_pCurPath->getCurTargetPoint(m_dwCurPos)) && ccpDistance(getPosition(), *pTarget) < m_fBuffArrive)
    {
        m_pCurPath->arriveCurTargetPoint(m_dwCurPos);
    }
    
    if (!isDoingOr(kAttacking) && m_pCurPath)
    {
        moveAlongPath(m_pCurPath, false, m_bWithHostility, m_fBuffArrive);
    }

    CGameUnit::onTick(fDt);
}

void CPathGameUnit::moveAlongPath( CUnitPath* pPath, bool bRestart /*= false*/, bool bWithHostility /*= true*/, float fBuffArrive /*= FLT_EPSILON*/ )
{
    if (pPath != m_pCurPath)
    {
        CC_SAFE_RELEASE(m_pCurPath);
        CC_SAFE_RETAIN(pPath);
        m_pCurPath = pPath;
    }
    
    if (!m_pCurPath)
    {
        return;
    }

    if (bRestart)
    {
        m_dwCurPos = 0;
    }
    if (m_bWithHostility != bWithHostility)
    {
        m_bWithHostility = bWithHostility;
    }
    if (m_fBuffArrive != fBuffArrive)
    {
        m_fBuffArrive = MAX(FLT_EPSILON, fBuffArrive);
    }
    
    const CCPoint* pTarget = m_pCurPath->getCurTargetPoint(m_dwCurPos);
    if (pTarget)
    {
        moveTo(*pTarget, m_bWithHostility);
    }
}

void CPathGameUnit::setMovingWithHostility( bool bWithHostility /*= true*/ )
{
    m_bWithHostility = bWithHostility;
}

bool CPathGameUnit::isMovingWithHostility() const
{
    return m_bWithHostility;
}
