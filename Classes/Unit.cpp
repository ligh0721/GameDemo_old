#include "CommInc.h"

#include "GameDisplay.h"
#include "Skill.h"
#include "Unit.h"
#include "Package.h"
#include "Action.h"
#include "MissionInfo.h"
#include "GameCtrl.h"
#include "GameFile.h"
#include "UnitInfo.h"
#include "SkillInfo.h"
#include "WHomeScene.h"
#include "SkillInfo.h"


void COnTickCallback::onDamaged(CUnit *pUnit, CAttackData *pAttack, CUnit *pSource, uint32_t dwTriggerMask)
{
}

void COnTickCallback::onDie(CUnit *pUnit)
{
}

void COnTickCallback::onTick(CUnit *pUnit, float fDt)
{
}


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
    uint32_t dwOldMaxExp;
    m_dwExp += dwExp;
    while (m_dwExp >= m_dwMaxExp)
    {
        ++m_dwLvl;
        dwOldMaxExp = m_dwMaxExp;
        updateMaxExp();
        m_dwExp -= dwOldMaxExp;
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

CCObject* CAttackBuff::copyWithZone( CCZone* pZone )
{
    return CAttackBuff::create(dynamic_cast<CBuffSkill*>(m_pBuff->copy()), m_iProbability);
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

CCObject* CAttackData::copyWithZone( CCZone* pZone )
{
    CAttackData* pAd = CAttackData::create();
    pAd->setAngle(getAngle());
    pAd->setAttack(*this);
    pAd->m_oArrBuff.addObjectsFromArray(dynamic_cast<CCArray*>(m_oArrBuff.copy()));
    return pAd;
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
, m_pOnTick(NULL)
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
    //m_oArrOnTickDelChain.init();
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
        if (M_RAND_HIT(pAtkBuff->m_iProbability))
        {
            addBuff(pAtkBuff->m_pBuff);
        }
    }

    transformDamageByAttribute(pAttack);
    float fDamage = 0;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; i++)
    {
        fDamage += calcDamage((CAttackValue::ATTACK_TYPE)i, pAttack->getAttack((CAttackValue::ATTACK_TYPE)i), m_eArmorType, m_fBaseArmorValue);
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
}

void CUnit::onHpChange( float fChanged )
{
    triggerOnHpChange(fChanged);
}

void CUnit::onTick(float fDt)
{
    triggerOnTick(fDt);
    if (m_pOnTick)
    {
        m_pOnTick->onTick(this, fDt);
    }
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
/*
void CUnit::delFromOnTickTriggerLater(CSkill* pSkill)
{
    delFromOnTickTrigger(pSkill);
    //m_oArrOnTickDelChain.addObject(pSkill);
}
*/
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
    CCArray oArrOnTick;
    oArrOnTick.initWithArray(&m_oArrOnTickChain);
    CCARRAY_FOREACH(&oArrOnTick, pObj)
    {
        dynamic_cast<CSkill*>(pObj)->onUnitTick(fDt);
    }
    /*
    if (m_oArrOnTickDelChain.count())
    {
        CCAssert(false, "");
        m_oArrOnTickChain.removeObjectsInArray(&m_oArrOnTickDelChain);
        m_oArrOnTickDelChain.removeAllObjects();
    }
    */
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
            coverBuff(pOldBuff);
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
    /*
    if (!bAfterTriggerLoop)
    {
        m_oArrOnTickChain.removeObjectsInArray(&m_oArrOnTickDelChain);
        m_oArrOnTickDelChain.removeAllObjects();
    }
    */
}

void CUnit::coverBuff( CBuffSkill* pBuff )
{
    pBuff->onSkillCover();
    pBuff->setOwner(NULL);
    m_oArrBuff.removeObject(pBuff);
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
    m_oArrOnAttackTargetChain.removeAllObjects();
    m_oArrOnAttackedChain.removeAllObjects();
    m_oArrOnDamagedSurfaceChain.removeAllObjects();
    m_oArrOnDamagedInnerChain.removeAllObjects();
    m_oArrOnDamageTargetChain.removeAllObjects();
    m_oArrOnHpChangeChain.removeAllObjects();
    m_oArrOnReviveChain.removeAllObjects();
    m_oArrOnDieChain.removeAllObjects();
    m_oArrOnTickChain.removeAllObjects();
    //m_oArrOnTickDelChain.removeAllObjects();
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

float CUnit::calcDamage( CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue )
{
    float fRet;
    if (fArmorValue > 0)
    {
        fRet = fArmorValue * 0.06;
        fRet = 1 - fRet / (fRet + 1);
    }
    else
    {
        fRet = 2 - pow(0.94f, -fArmorValue);
    }
    fRet *= fAttackValue * g_afAttackArmorTable[eArmorType][eAttackType];
    return fRet;
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

const CGameUnit::UNIT_MOVE_PARAMS CGameUnit::CONST_DEFAULT_MOVE_PARAMS;

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
    setFixed(false);
    setHostilityRange(0);
    setRewardGold(0);
    setRewardExp(0);
    setExAttackRandomRange(0.000);
    setStatus(kNormal);
    m_pRes = NULL;
    setUnitLayer(NULL);
    m_pMovePath = NULL;
    setPathCurPos(0);
    setPathIntended(false);
    m_fPathBufArrive = FLT_EPSILON;
    setToCastSkill(NULL);
    setCastingSkill(NULL);
    m_iSuspendRef = 0;

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
    setFixed(false);
    setHostilityRange(0);
    setRewardGold(0);
    setRewardExp(0);
    setExAttackRandomRange(0.000);
    setStatus(kNormal);
    m_pRes = NULL;
    setUnitLayer(NULL);
    m_pMovePath = NULL;
    setPathCurPos(0);
    setPathIntended(false);
    m_fPathBufArrive = FLT_EPSILON;
    setToCastSkill(NULL);
    setCastingSkill(NULL);
    m_iSuspendRef = 0;

    return true;
}

bool CGameUnit::initWithInfo( const CUnitInfo& roUnitInfo )
{
    M_DEF_PM(pPm);
    CGameUnit::initWithName(roUnitInfo.m_sName.c_str(), roUnitInfo.m_oAnchor);
    setHalfOfWidth(roUnitInfo.m_fHalfOfWidth);
    setHalfOfHeight(roUnitInfo.m_fHalfOfHeight);
    m_oSprite.setScale(roUnitInfo.m_fScale);
    m_vecAniInfo = roUnitInfo.m_vecAniInfo;
    m_vecAttackAniIndex = roUnitInfo.m_vecAttackAni;

    setBaseMoveSpeed(roUnitInfo.m_fBaseMoveSpeed);
    setBaseAttackInterval(roUnitInfo.m_fBaseAttackInterval);
    setAttackMinRange(roUnitInfo.m_fAttackMinRange);
    setAttackRange(roUnitInfo.m_fAttackRange);
    setHostilityRange(roUnitInfo.m_fHostilityRange);
    setWeaponType(roUnitInfo.m_eWeaponType);
    setTemplateProjectile(pPm->getProjectile(roUnitInfo.m_iProjectileKey));
    setProjectileMoveSpeed(roUnitInfo.m_fProjectileMoveSpeed);
    setProjectileScale(roUnitInfo.m_fProjectileScale);
    setProjectileMaxOffsetY(roUnitInfo.m_fProjectileMaxOffsetY);
    setProjectileBirthOffset(ccp(roUnitInfo.m_fProjectileBirthOffsetX, roUnitInfo.m_fProjectileBirthOffsetY));
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
    CCSpriteFrame* pF = CGameManager::sharedGameManager()->getUnitFrame(m_sUnit.c_str(), pFrame);
    if (pF)
    {
        m_oSprite.setDisplayFrame(pF);
    }
}

void CGameUnit::setDefaultFrame()
{
    setFrame(NULL);
}

void CGameUnit::prepareMoveAnimation( const char* pAnimation, float fDelay )
{
    prepareAnimation(kAnimationMove, pAnimation, fDelay, 0.0);
}

void CGameUnit::prepareAttackAnimation( int iAttackAniCount, ANIMATION_INDEX eAnimation1, const char* pAnimation1, float fDelay1, float fEffect1, ... )
{
    m_vecAttackAniIndex.resize(iAttackAniCount);

    prepareAnimation(eAnimation1, pAnimation1, fDelay1, fEffect1);
    m_vecAttackAniIndex[0] = eAnimation1;

    va_list argv;
    va_start(argv, fDelay1);
    for (int i = 1; i < iAttackAniCount; ++i)
    {
        eAnimation1 = (ANIMATION_INDEX)va_arg(argv, int);
        pAnimation1 = (const char*)va_arg(argv, int);
        fDelay1 = (float)va_arg(argv, double);
        fEffect1 = (float)va_arg(argv, double);
        prepareAnimation(eAnimation1, pAnimation1, fDelay1, fEffect1);
        m_vecAttackAniIndex[i] = eAnimation1;
    }

    va_end(argv);
}

void CGameUnit::prepareDieAnimation( const char* pAnimation, float fDelay )
{
    prepareAnimation(kAnimationDie, pAnimation, fDelay, 0.0);
}

void CGameUnit::prepareAnimation( ANIMATION_INDEX eAnimation, const char* pAnimation, float fDelay, float fEffect )
{
    int iIndex = (int)eAnimation;
    if (iIndex >= (int)m_vecAniInfo.size())
    {
        m_vecAniInfo.resize(iIndex + 1);
    }
    ANIMATION_INFO& rAni = m_vecAniInfo[eAnimation];
    rAni.sAnimation = pAnimation;
    rAni.fDelay = fDelay;
    rAni.fEffect = fEffect;
}

void CGameUnit::setAnimation( const char* pAnimation, float fDelay, int iRepeat, float fSpeed, ACTION_TAG eTag, CCFiniteTimeAction* pEndAction )
{
    if (m_oSprite.getActionByTag(eTag))
    {
        return;
    }
    M_DEF_GM(pGm);
    CCAnimation* pAni = pGm->getUnitAnimation(getName(), pAnimation);
    if (!pAni)
    {
        CCLOG("%s/%s NOT FOUND", getName(), pAnimation);
        return;
    }
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
    ANIMATION_INFO& rAni = m_vecAniInfo[eAnimation];
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

void CGameUnit::moveTo( const CCPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/ )
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
    if (roMoveParams.bAutoFlipX)
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
    // 突发移动指令，打断旧移动，打断攻击，打断施法
    if (isDoingOr(kMoving))
    {
        m_oSprite.stopActionByTag(kActMoveTo);
    }
    if (isDoingOr(kAttacking) && roMoveParams.bCancelAttack)
    {
        stopAttack();
    }
    if (isDoingOr(kCasting) && roMoveParams.bCancelCast)
    {
        stopCast();
    }
    m_oSprite.runAction(pActMoveTo);
    if (!isDoingOr(kSpinning))
    {
        setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    }

    startDoing(kMoving);
    if (roMoveParams.bIntended)
    {
        startDoing(kIntended);
    }
    else
    {
        endDoing(kIntended);
    }
}

void CGameUnit::followTo( int iTargetKey, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/ )
{
    if (isDead() || isFixed())
    {
        return;
    }
    M_DEF_GM(pGm);
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(iTargetKey);
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
    if (roMoveParams.bAutoFlipX)
    {
        turnTo(roOrg.x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = pGm->getDistance(roOrg, roPos) / fMoveSpeed;
    CCActionInterval* pSeq = CCSequence::createWithTwoActions(
                                                              CCMoveToNode::create(fDur, pTarget->getSprite(), true, roMoveParams.fMaxOffsetY, 1, pTarget->getHalfOfHeight()),
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
    if (isDoingOr(kAttacking) && roMoveParams.bCancelAttack)
    {
        stopAttack();
    }
    if (isDoingOr(kCasting) && roMoveParams.bCancelCast)
    {
        stopCast();
    }
    m_oSprite.runAction(pActMoveTo);
    if (!isDoingOr(kSpinning))
    {
        setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    }

    startDoing(kMoving);
    if (roMoveParams.bIntended)
    {
        startDoing(kIntended);
    }
    else
    {
        endDoing(kIntended);
    }
}

void CGameUnit::moveAlongPath( CUnitPath* pPath, bool bIntended /*= true*/, bool bRestart /*= false*/, float fBufArrive /*= 5.0*/ )
{
    if (pPath != m_pMovePath)
    {
        CC_SAFE_RETAIN(pPath);
        CC_SAFE_RELEASE(m_pMovePath);
        m_pMovePath = pPath;
    }

    if (!m_pMovePath)
    {
        return;
    }

    if (bRestart)
    {
        m_dwPathCurPos = 0;
    }

    if (m_fPathBufArrive != fBufArrive)
    {
        m_fPathBufArrive = MAX(FLT_EPSILON, fBufArrive);
    }

    setPathIntended(bIntended);

    const CCPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
    if (pTarget)
    {
        UNIT_MOVE_PARAMS oMp;
        oMp.bIntended = m_bPathIntended;
        moveTo(*pTarget, oMp);
    }
}

void CGameUnit::setPathIntended( bool bPathIntended /*= true*/ )
{
    m_bPathIntended = bPathIntended;
}

bool CGameUnit::isPathIntended() const
{
    return m_bPathIntended;
}

void CGameUnit::stopMove()
{
    m_oSprite.stopActionByTag(kActMoveTo);
    m_oSprite.stopActionByTag(kActMove);
    endDoing(kMoving | kIntended);
    if (!isDoingOr(kSpinning))
    {
        setDefaultFrame();
    }
}

const CCPoint& CGameUnit::getLastMoveToTarget() const
{
    return m_oLastMoveToTarget;
}

const CCPoint& CGameUnit::getLastMoveToTarget()
{
    return m_oLastMoveToTarget;
}

void CGameUnit::onActMoveEnd( CCNode* pNode )
{
    endDoing(kIntended);
    if (isDoingOr(kAttacking))
    {
        attack(getLastAttackTarget(), isDoingOr(kIntended));
    }
    else if (isDoingOr(kCasting))
    {
        cast();
    }
    else
    {
        stopMove();
    }
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
    float fDur = pActSeq->getDuration() / fDelta;
    pActSeq->setDuration(fDur);
    dynamic_cast<CCDelayTime*>(pActSeq->getActionOne())->setDuration(fDur);
}

void CGameUnit::attack( int iTargetKey, bool bIntended /*= true*/)
{
    if (isDead() || iTargetKey == getKey())
    {
        return;
    }
    float fRealAttackInterval = getRealAttackInterval();
    //M_DEF_GM(pGm);
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(iTargetKey);
    if (!pTarget || pTarget->isDead() || pTarget->getStatus() == kNoAttacked)
    {
        // 原目标已不存在
        if (isDoingOr(kAttacking) && !m_oSprite.getActionByTag(kActAttack))
        {
            // 攻击中，且没有施展动作，可能是原地打，也可能是追击中
            endDoing(kAttacking | kIntended);
            //m_iLastAttackTarget = 0;
        }
        m_iLastAttackTarget = 0;
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

        // 位置符合，可以立即发动攻击，突发攻击指令，打断移动，打断旧攻击，打断施法
        if (isDoingOr(kMoving))
        {
            stopMove();
        }
        if (isDoingOr(kCasting))
        {
            stopCast();
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

        // 设置攻击标志位(确定攻击意识)
        startDoing(kAttacking);
        if (bIntended)
        {
            startDoing(kIntended);
        }
        else
        {
            endDoing(kIntended);
        }
        if (m_fAttackCD > 0 || isDoingOr(kSpinning))
        {
            return;
        }

        // 发动攻击动作
        m_fAttackCD = fRealAttackInterval;
        float fDelta = getBaseAttackInterval() / fRealAttackInterval;
        ANIMATION_INDEX eAni = m_vecAttackAniIndex[rand() % m_vecAttackAniIndex.size()];
        ANIMATION_INFO& rAni = m_vecAniInfo[eAni];
        setAnimation(eAni, 0, fDelta, kActAttack, CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActAttackEnd)));
        CCAction* pAct = CCSequenceEx::createWithTwoActions(CCDelayTime::create(rAni.fEffect / fDelta), CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActAttackEffect)));
        pAct->setTag(kActAttackEffect);
        m_oSprite.runAction(pAct);
        return;
    }

    // 由于位置原因，无法进行攻击
    if (isFixed())
    {
        // 如果是建筑，只需丢失攻击目标即可
        m_iLastAttackTarget = 0;
        endDoing(kAttacking | kIntended);
        return;
    }

    // 进行攻击校正

    if (getLastAttackTarget() == iTargetKey)
    {
        if (isDoingAnd(kAttacking | kMoving))
        {
            // 位置校正中
            if (!checkAttackDistance(getLastMoveToTarget(), pTarget))
            {
                // 攻击目标未变，但修正位置已过期
                //if (getWeaponType() == kWTDelayed) CCLOG("!!!!!!!!!");
                moveToAttackPosition(pTarget, bIntended);
            }
            return;
        }

        if (m_oSprite.getActionByTag(kActAttack) || m_oSprite.getActionByTag(kActAttackEffect))
        {
            return;
        }
    }

    m_iLastAttackTarget = iTargetKey;
    moveToAttackPosition(pTarget, bIntended);
}

void CGameUnit::stopAttack()
{
    m_oSprite.stopActionByTag(kActAttack);
    m_oSprite.stopActionByTag(kActAttackEffect);
    m_iLastAttackTarget = 0;
    endDoing(kAttacking);
    if (!isDoingOr(kSpinning))
    {
        setDefaultFrame();
    }
}

void CGameUnit::onActAttackEffect( CCNode* pNode )
{
    CAttackData* pAtk = CAttackData::create();
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        pAtk->setAttack((CAttackValue::ATTACK_TYPE)i, getRealAttackValue((CAttackValue::ATTACK_TYPE)i));
    }
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(getLastAttackTarget());
    if (!pTarget || pTarget->isDead())
    {
        return;
    }
    CProjectile* pProj;
    pAtk = attackAdv(pAtk, pTarget);
    if (getDistance(pTarget) > getAttackRange() + getHalfOfWidth() + pTarget->getHalfOfWidth() + CONST_MAX_ATTACK_BUFFER_RANGE)
    {
        return;
    }
    switch (getWeaponType())
    {
        case kWTClosely:
        case kWTInstant:
            if (!getTemplateProjectile())
            {
                pTarget->damagedAdv(pAtk, this);
            }
            else
            {
                pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
                pProj->fireInstant(getUnitLayer(), this, this, pTarget, pAtk, getProjectileScale(), getProjectileBirthOffset());
                /*
                CCUnitLayer* pLayer = dynamic_cast<CCUnitLayer*>(m_oSprite.getParent());
                pLayer->addProjectile(pProj);
                pProj->setProjectileBirthOffset(getProjectileBirthOffset());
                pProj->setAttackData(pAtk);
                pProj->setOwner(getKey());
                pProj->setStart(getKey());
                pProj->setTarget(getLastAttackTarget());
                pProj->getSprite()->setScale(getProjectileScale());
                pProj->setPosition(pTarget->getPosition());
                pProj->onDie();
                */
            }
            break;

        case kWTDelayed:
            pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
            pProj->fireFolow(getUnitLayer(), this, this, pTarget, pAtk, getProjectileScale(), getProjectileBirthOffset(), getProjectileMoveSpeed(), getProjectileMaxOffsetY());
            /*
            getUnitLayer()->addProjectile(pProj);
            pProj->setAttackData(pAtk);
            pProj->setOwner(getKey());
            pProj->setStart(getKey());
            pProj->setTarget(getLastAttackTarget());
            pProj->setBaseMoveSpeed(getProjectileMoveSpeed());
            const CCPoint& roPos1 = pProj->getPosition();
            const CCPoint& roPos2 = pTarget->getPosition();
            float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(roPos2, roPos1)));
            pProj->getSprite()->setScale(getProjectileScale());
            pProj->getSprite()->setRotation(fA);
            //float fOffsetX = getHalfOfWidth();
            pProj->setProjectileBirthOffset(getProjectileBirthOffset());
            pProj->setPosition(ccpAdd(getPosition(), ccp(getSprite()->isFlipX() ? -pProj->getProjectileBirthOffset().x : pProj->getProjectileBirthOffset().x, pProj->getProjectileBirthOffset().y)));
            //pProj->setHalfOfWidth(11);
            //pProj->setHalfOfHeight(13);
            UNIT_MOVE_PARAMS oMp;
            oMp.bIntended = false;
            oMp.bAutoFlipX = false;
            oMp.fMaxOffsetY = getProjectileMaxOffsetY();
            pProj->followTo(getLastAttackTarget(), oMp);
            */
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
    if (getWeaponType() == kWTClosely && abs(roPos.y - roPos2.y) > CONST_MAX_CLOSE_ATTACK_Y_RANGE)
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

void CGameUnit::moveToAttackPosition( CGameUnit* pTarget, bool bIntended )
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

    UNIT_MOVE_PARAMS oMp;
    oMp.bIntended = false;
    oMp.bCancelAttack = false;
    if (getWeaponType() == kWTClosely)
    {
        // 近战攻击位置修正
        moveTo(ccp(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), oMp);
    }
    else
    {
        // 远程攻击位置修正
        float fA = -ccpToAngle(ccpSub(roPos1, roPos2));
        moveTo(ccpAdd(roPos2, ccp(cos(fA) * fDis, sin(-fA) * fDis)), oMp);
    }

    startDoing(kAttacking);
    if (bIntended)
    {
        startDoing(kIntended);
    }
    else
    {
        endDoing(kIntended);
    }
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
    ++m_iSuspendRef;
    startDoing(kSuspended);
    stopMove();
    stopAttack();
}

void CGameUnit::resume()
{
    if (m_iSuspendRef > 0)
    {
        --m_iSuspendRef;
        if (!m_iSuspendRef)
        {
            endDoing(kSuspended);
        }
    }
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

    if (isDoingOr(kCasting))
    {
        cast();
        return;
    }

    // 路径逻辑
    if (m_pMovePath)
    {
        const CCPoint* pTarget;
        if ((pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos)) && ccpDistance(getPosition(), *pTarget) < m_fPathBufArrive)
        {
            m_pMovePath->arriveCurTargetPoint(m_dwPathCurPos);
        }

        if (!isDoingOr(kAttacking))
        {
            moveAlongPath(m_pMovePath, isPathIntended(), false, m_fPathBufArrive);
        }
    }

    if (isDoingOr(kAttacking))
    {
        attack(getLastAttackTarget(), isDoingOr(kIntended));
    }
    else if (!isDoingOr(kIntended) && (isDoingNothing() || isDoingOr(kMoving | kSpinning)))
    {
        CGameUnit* pUnit = getUnitLayer()->getUnits()->getNearestUnitInRange(getPosition(), getHostilityRange(), CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(this));
        if (pUnit)
        {
            attack(pUnit->getKey(), false);
        }
    }
}

void CGameUnit::onDie()
{
    getSprite()->stopAllActions();
    getUnitLayer()->onUnitDie(this);

    setForceResource(NULL);
    stopMove();
    stopAttack();
    stopSpin();
    setAnimation(kAnimationDie, 1, 1, kActDie, CCSequence::create(CCDelayTime::create(5), CCFadeOut::create(1), CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActDieEnd)), NULL));
    CUnit::onDie();
}

void CGameUnit::onDamaged( CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask )
{
    CUnit::onDamaged(pAttack, pSource, dwTriggerMask);

    CGameUnit* pSrc = dynamic_cast<CGameUnit*>(pSource);
    //CCLOG("dis:%.2f %.2f", getDistance(pSrc), getHostilityRange());
    CGameUnit* pLu;
    if ( pSrc && !isDoingOr(kIntended) && (
                                           ( isDoingOr(kAttacking) && ( pLu = getUnitLayer()->getUnitByKey(getLastAttackTarget()) ) && getDistance(pLu) > getHostilityRange() ) ||
                                           isDoingNothing() ||
                                           ( isDoingOr(kMoving | kSpinning) && !isDoingOr(kAttacking))
                                           ))
    {
        attack(pSrc->getKey(), false);
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

void CGameUnit::turnTo( const CCPoint& roPos )
{
    if (isFixed())
    {
        return;
    }
    m_oSprite.setFlipX(roPos.x < getPosition().x);
}

void CGameUnit::onActDieEnd( CCNode* pNode )
{
    //m_oSprite.removeFromParentAndCleanup(true);
    getUnitLayer()->moveUnitToDustbin(this);
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
    CC_SAFE_RETAIN(pRes);
    CC_SAFE_RELEASE(m_pRes);
    m_pRes = pRes;
}

CForceResouce* CGameUnit::getForceResource()
{
    return m_pRes;
}

CUnitPath* CGameUnit::getMovePath()
{
    return m_pMovePath;
}
bool CGameUnit::cast()
{
    // test
    CActiveSkill* pSkill = getToCastSkill(); // 只要skill没有cast，就非NULL
    if (!pSkill)
    {
        return false;
    }

    if (isDead())
    {
        getUnitLayer()->endOrderUnitToCast();
        return false;
    }

    if (isDoingOr(kSuspended))
    {
        // 眩晕中，就退出
        //m_iLastAttackTarget = iTargetKey;
        return false;
    }

    CCPoint roPos1 = getPosition();
    CCPoint roPos2 = pSkill->updateTargetUnitPoint();
    switch (pSkill->getCastTargetType()) // break 代表可以施法
    {
        case CActiveSkill::kNoTarget:
            // 原地释放技能
            break;

        case CActiveSkill::kUnitTarget:
        case CActiveSkill::kPointTarget:
            if (checkCastDistance(roPos1))
            {
                break;
            }

            // 由于位置原因，无法进行攻击
            if (isFixed())
            {
                // 如果是建筑，只需丢失施法目标即可
                setToCastSkill(NULL);
                endDoing(kCasting);
                return false;
            }

            if (getToCastSkill() == getCastingSkill())
            {
                // 进行施法校正
                if (isDoingAnd(kCasting | kMoving))
                {
                    // 位置校正中
                    if (!checkCastDistance(getLastMoveToTarget()))
                    {
                        // 但修正位置已过期
                        moveToCastPosition();
                    }
                    return false;
                }

                if (m_oSprite.getActionByTag(kActCast) || m_oSprite.getActionByTag(kActCastEffect))
                {
                    //stopCast();
                    return false;
                }
            }


            moveToCastPosition();
            return false;
            break;
    }

    // 可以将对该目标进行施法

    // 位置符合，可以立即发动施法，突发施法指令，打断移动，打断攻击，打断旧施法
    if (isDoingOr(kMoving))
    {
        stopMove();
    }

    if (isDoingOr(kAttacking))
    {
        stopAttack();
    }

    if (getToCastSkill() == getCastingSkill() && m_oSprite.getActionByTag(kActCast))
    {
        // 新施法就是正在进行的施法，直接返回
        return true;
    }

    if (isDoingOr(kCasting))
    {
        stopCast();
    }

    if (pSkill->getCastTargetType() != CActiveSkill::kNoTarget)
    {
        turnTo(roPos2);
    }

    // 设置施法标志位(确定施法意识)
    startDoing(kCasting | kIntended);

    // 发动攻击动作
    if (!pSkill->canCast())
    {
        return false;
    }

    setCastingSkill(getToCastSkill());
    ANIMATION_INDEX eAni = pSkill->getCastAniIndex();
    if (eAni < 0)
    {
        onActCastEnd(getSprite());
        onActCastEffect(getSprite());
    }
    else
    {
        setAnimation(eAni, 0, 1, kActCast, CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActCastEnd)));
        CCAction* pAct = CCSequenceEx::createWithTwoActions(CCDelayTime::create(m_vecAniInfo[eAni].fEffect), CCCallFuncN::create(this, callfuncN_selector(CGameUnit::onActCastEffect)));
        pAct->setTag(kActCastEffect);
        m_oSprite.runAction(pAct);
    }

    return true;
}

void CGameUnit::stopCast()
{
    m_oSprite.stopActionByTag(kActCast);
    m_oSprite.stopActionByTag(kActCastEffect);
    endDoing(kCasting | kIntended);
    if (!isDoingOr(kSpinning))
    {
        setDefaultFrame();
    }
}

void CGameUnit::onActCastEffect( CCNode* pNode )
{
    CActiveSkill* pSkill = getToCastSkill();
    switch (pSkill->getCastTargetType())
    {
        case CActiveSkill::kPointTarget:
            break;

        case CActiveSkill::kUnitTarget:
            CGameUnit* t = getUnitLayer()->getUnitByKey(pSkill->getTargetUnit());
            if (!t)
            {
                return;
            }
            const CCPoint& roPos2 = pSkill->updateTargetUnitPoint();
            if (getDistance(roPos2) > pSkill->getCastRange() + getHalfOfWidth() + pSkill->getTargetUnitHalfOfWidth() + CGameUnit::CONST_MAX_ATTACK_BUFFER_RANGE)
            {
                return;
            }
            break;
    }
    pSkill->cast();
}

void CGameUnit::onActCastEnd( CCNode* pNode )
{
    endDoing(kCasting | kIntended);
    setDefaultFrame();
}

bool CGameUnit::checkCastDistance( const CCPoint& roPos )
{
    CActiveSkill* pSkill = getToCastSkill();
    CCPoint roPos2;
    CGameUnit* pTarget = NULL;
    switch (pSkill->getCastTargetType())
    {
        case CActiveSkill::kUnitTarget:
        case CActiveSkill::kPointTarget:
            roPos2 = pSkill->getTargetPoint();
            break;

        default:
            CCAssert(false, "err cast target type");
            return true;
    }

    if (pSkill->getWeaponType() == CGameUnit::kWTClosely && abs(roPos.y - roPos2.y) > CONST_MAX_CLOSE_ATTACK_Y_RANGE)
    {
        return false;
    }

    M_DEF_GM(pGm);
    float fDis = pGm->getDistance(roPos, roPos2) - getHalfOfWidth() - pSkill->getTargetUnitHalfOfWidth();
    fDis = MAX(0.5, fDis);
    if (fDis > pSkill->getCastRange())
    {
        return false;
    }

    return true;
}

void CGameUnit::moveToCastPosition()
{
    if (isFixed())
    {
        return;
    }
    m_oSprite.stopActionByTag(kActCast);
    m_oSprite.stopActionByTag(kActCastEffect);

    CActiveSkill* pSkill = getToCastSkill();
    float fDis = pSkill->getTargetUnitHalfOfWidth() + getHalfOfWidth() + pSkill->getCastRange() * 0.5;
    const CCPoint& roPos1 = getPosition();
    const CCPoint& roPos2 = pSkill->getTargetPoint();

    UNIT_MOVE_PARAMS oMp;
    oMp.bIntended = false;
    oMp.bCancelCast = false;
    if (pSkill->getWeaponType() == CGameUnit::kWTClosely)
    {
        // 近战攻击位置修正
        moveTo(ccp(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), oMp);
    }
    else
    {
        // 远程攻击位置修正
        float fA = -ccpToAngle(ccpSub(roPos1, roPos2));
        moveTo(ccpAdd(roPos2, ccp(cos(fA) * fDis, sin(-fA) * fDis)), oMp);
    }

    startDoing(kCasting | kIntended);
}

void CGameUnit::stopSpin()
{
    m_oSprite.stopActionByTag(kActSpin);
    endDoing(kSpinning);
    if (isDoingOr(kMoving))
    {
        setAnimation(kAnimationMove, -1, getRealMoveSpeed() / getBaseMoveSpeed(), kActMove, NULL);
    }
    else if (isDoingNothing())
    {
        setDefaultFrame();
    }
}

void CGameUnit::say( const char* pMsg )
{

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
    setStart(0);
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
    setStart(0);
    m_iTarget = 0;
    setOffsetZ(0);
    setGeneration(0);
    setTargetObj(NULL);
    setTargetFun(NULL);
    setProjectileType(kFollow);
    return CGameUnit::initWithName(pProjectile, roAnchor);
}


CAttackData* CProjectile::getAttackData() const
{
    return m_pAttackData;
}

void CProjectile::setAttackData( CAttackData* pAttackData )
{
    CC_SAFE_RETAIN(pAttackData);
    CC_SAFE_RELEASE(m_pAttackData);
    m_pAttackData = pAttackData;
}

void CProjectile::onActMoveEnd( CCNode* pNode )
{
    CGameUnit::onActMoveEnd(pNode);
    //setAnimation(kAnimationDie, 1, 0.1, kActDie, NULL);
    onDie();
}

void CProjectile::onDie()
{
    if (m_eProjectileType == kRange)
    {
        m_oSprite.getScheduler()->unscheduleSelector(schedule_selector(CProjectile::onMovingTick), this);
        setAnimation(kAnimationDie, 1, 1, kActDie, CCCallFuncN::create(this, callfuncN_selector(CProjectile::onActDieEnd)));
        return;
    }

    M_DEF_GM(pGm);
    CCAnimation* pAni;
    CCLightning* pLigh;
    CCAction* pAct;
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(getTarget());
    CGameUnit* pOwner = getUnitLayer()->getUnitByKey(getOwner());
    CGameUnit* pStart = getUnitLayer()->getUnitByKey(getStart());

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
            pAni = pGm->getUnitAnimation(getName(), m_vecAniInfo[kAnimationDie].sAnimation.c_str());
            pAni->setDelayPerUnit(m_vecAniInfo[kAnimationDie].fDelay);
            pLigh = CCLightning::create(pAni, pStart->getSprite(), pTarget->getSprite(), getProjectileBirthOffset(), pTarget->getHalfOfHeight());
            pLigh->fixTargetPosition(&m_oSprite);
            pAct = CCSequence::createWithTwoActions(pLigh, CCCallFuncN::create(this, callfuncN_selector(CProjectile::onActDieEnd)));
            pAct->setTag(kActDie);
            m_oSprite.runAction(pAct);
            break;

        case kRange:

            break;
    }

    CUnit::onDie(); // Don't call CGameUnit::onDie()
}

void CProjectile::onActDieEnd( CCNode* pNode )
{
    //m_oSprite.removeFromParentAndCleanup(true);
    M_DEF_GM(pGm);
    if (m_pTargetObj != NULL
        && m_pTargetFun != NULL) {

        (m_pTargetObj->*m_pTargetFun)(this);
    }
    getUnitLayer()->moveProjectileToDustbin(this);
}

CCObject* CProjectile::copyWithZone( CCZone* pZone )
{
    CProjectile* pProj = CProjectile::createWithName(getName());
    pProj->m_vecAniInfo = m_vecAniInfo;
    pProj->setProjectileType(getProjectileType());
    pProj->setBaseMoveSpeed(getBaseMoveSpeed());
    return pProj;
}

void CProjectile::setTarget( int iTarget )
{
    setTarget(getUnitLayer()->getUnitByKey(iTarget));
}

void CProjectile::setTarget( CGameUnit* pTarget )
{
    m_iTarget = pTarget->getKey();
    setOffsetZ(pTarget->getHalfOfHeight());
    m_oTarget = pTarget->getPosition();
}

void CProjectile::setTarget( const CCPoint& roTarget )
{
    m_oTarget = roTarget;
}

int CProjectile::getTarget() const
{
    return m_iTarget;
}

const CCPoint& CProjectile::getTargetPoint() const
{
    return m_oTarget;
}

void CProjectile::fireInstant( CCUnitLayer* pLayer, int iOwner, int iStart, int iTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset )
{
    CGameUnit* pOwner = pLayer->getUnitByKey(iOwner);
    CGameUnit* pStart = pLayer->getUnitByKey(iStart);
    CGameUnit* pTarget = pLayer->getUnitByKey(iTarget);
    fireInstant(pLayer, pOwner, pStart, pTarget, pAtk, fScale, roBirthOffset);
}

void CProjectile::fireInstant( CCUnitLayer* pLayer, CGameUnit* pOwner, CGameUnit* pStart, CGameUnit* pTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset )
{
    pLayer->addProjectile(this);
    setAttackData(pAtk);
    setOwner(pOwner->getKey());
    setStart(pStart->getKey());
    setTarget(pTarget->getKey());
    getSprite()->setScale(fScale);
    setProjectileBirthOffset(roBirthOffset);
    setPosition(pTarget->getPosition());
    onDie();
}

void CProjectile::fireFolow( CCUnitLayer* pLayer, int iOwner, int iStart, int iTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset, float fMoveSpeed, float fMaxOffsetY )
{
    CGameUnit* pOwner = pLayer->getUnitByKey(iOwner);
    CGameUnit* pStart = pLayer->getUnitByKey(iStart);
    CGameUnit* pTarget = pLayer->getUnitByKey(iTarget);
    fireFolow(pLayer, pOwner, pStart, pTarget, pAtk, fScale, roBirthOffset, fMoveSpeed, fMaxOffsetY);
}

void CProjectile::fireFolow( CCUnitLayer* pLayer, CGameUnit* pOwner, CGameUnit* pStart, CGameUnit* pTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset, float fMoveSpeed, float fMaxOffsetY )
{
    pLayer->addProjectile(this);
    setAttackData(pAtk);
    setOwner(pOwner->getKey());
    setStart(pStart->getKey());
    setTarget(pTarget->getKey());
    setBaseMoveSpeed(fMoveSpeed);
    getSprite()->setScale(fScale);
    setProjectileBirthOffset(roBirthOffset);
    setProjectileMaxOffsetY(fMaxOffsetY);

    const CCPoint& roPos1 = pStart->getPosition();
    const CCPoint& roPos2 = pTarget->getPosition();
    float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(roPos2, roPos1)));
    getSprite()->setRotation(fA);
    //float fOffsetX = getHalfOfWidth();
    setPosition(ccpAdd(pStart->getPosition(), ccp(pStart->getSprite()->isFlipX() ? -roBirthOffset.x : roBirthOffset.x, roBirthOffset.y)));
    //pProj->setHalfOfWidth(11);
    //pProj->setHalfOfHeight(13);

    UNIT_MOVE_PARAMS oMp;
    oMp.bAutoFlipX = false;
    oMp.fMaxOffsetY = fMaxOffsetY;
    followTo(pTarget->getKey(), oMp);
}

void CProjectile::fireWave( CCUnitLayer* pLayer, int iOwner, int iStart, const CCPoint& roTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset, float fMoveSpeed, float fRange )
{
    CGameUnit* pOwner = pLayer->getUnitByKey(iOwner);
    CGameUnit* pStart = pLayer->getUnitByKey(iStart);
    fireWave(pLayer, pOwner, pStart, roTarget, pAtk, fScale, roBirthOffset, fMoveSpeed, fRange);
}

void CProjectile::fireWave( CCUnitLayer* pLayer, CGameUnit* pOwner, CGameUnit* pStart, const CCPoint& roTarget, CAttackData* pAtk, float fScale, const CCPoint& roBirthOffset, float fMoveSpeed, float fRange )
{
    pLayer->addProjectile(this);
    setAttackData(pAtk);
    setOwner(pOwner->getKey());
    setStart(pStart->getKey());
    setTarget(roTarget);
    setBaseMoveSpeed(fMoveSpeed);
    getSprite()->setScale(fScale);
    setProjectileBirthOffset(roBirthOffset);

    const CCPoint& roPos1 = pStart->getPosition();
    float fR = ccpToAngle(ccpSub(roTarget, roPos1));
    float fA = CC_RADIANS_TO_DEGREES(-fR);
    getSprite()->setRotation(fA);
    //float fOffsetX = getHalfOfWidth();
    setPosition(ccpAdd(pStart->getPosition(), ccp(pStart->getSprite()->isFlipX() ? -roBirthOffset.x : roBirthOffset.x, roBirthOffset.y)));
    //pProj->setHalfOfWidth(11);
    //pProj->setHalfOfHeight(13);

    UNIT_MOVE_PARAMS oMp;
    oMp.bAutoFlipX = false;
    if (m_eProjectileType == kRange)
    {
        m_oSprite.getScheduler()->scheduleSelector(schedule_selector(CProjectile::onMovingTick), this, 0.05, false);
    }

    if (fRange < 0)
    {
        moveTo(roTarget, oMp);
    }
    else
    {
        moveTo(ccpAdd(roPos1, ccp(cos(fR) * fRange, sin(fR) * fRange)), oMp);
    }
}

void CProjectile::onMovingTick( float fDt )
{
    CGameUnit* pOwner = getUnitLayer()->getUnitByKey(getOwner());
    if (!pOwner)
    {
        return;
    }
    CAttackData* pAtk;
    CCArray* pArrUnits = getUnitLayer()->getUnits()->getUnitsArray();
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(pArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (pUnit && pUnit->isEnemyOf(pOwner) && !pUnit->isDead() && pUnit->getDistance(getPosition()) - pUnit->getHalfOfWidth() - getHalfOfWidth() <= 0 && !m_mapDamaged[pUnit->getKey()])
        {
            m_mapDamaged[pUnit->getKey()] = true;
            pAtk = dynamic_cast<CAttackData*>(getAttackData()->copy());
            pUnit->damagedAdv(pAtk, pOwner);
        }
    }
}

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
    if (fRadius < FLT_EPSILON)
    {
        return true;
    }
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
        if (pUnit->getDistance(roPos) < fRadius && (!pBoolFunc || (pBoolFunc && pBoolFunc(pUnit, pParam))))
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
        if (!pBoolFunc || (pBoolFunc && pBoolFunc(pUnit, pParam)))
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
        if ((fDis = ccpDistance(pUnit->getPosition(), roPos)) < fRadius && fMinDis > fDis && (!pBoolFunc || (pBoolFunc && pBoolFunc(pUnit, pParam))))
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

void CUnitGroup::moveTo( const CCPoint& roPos, const CGameUnit::UNIT_MOVE_PARAMS& roMoveParams /*= CGameUnit::CONST_DEFAULT_MOVE_PARAMS*/)
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->moveTo(roPos, roMoveParams);
    }
}

void CUnitGroup::followTo( int iTargetKey, const CGameUnit::UNIT_MOVE_PARAMS& roMoveParams /*= CGameUnit::CONST_DEFAULT_MOVE_PARAMS*/)
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->followTo(iTargetKey, roMoveParams);
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

void CUnitGroup::attack( int iTargetKey, bool bIntended /*= true*/ )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->attack(iTargetKey, bIntended);
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

void CUnitGroup::moveAlongPath( CUnitPath* pPath, bool bIntended /*= true*/, bool bRestart /*= false*/, float fBufArrive /*= 5.0*/ )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (pUnit)
        {
            pUnit->moveAlongPath(pPath, bIntended, bRestart, fBufArrive);
        }
    }
}

void CUnitGroup::damagedAdv( CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pAttack = dynamic_cast<CAttackData*>(pAttack->copy());
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedAdv(pAttack, pSource, dwTriggerMask);
    }
}

void CUnitGroup::damagedMid( CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pAttack = dynamic_cast<CAttackData*>(pAttack->copy());
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedMid(pAttack, pSource, dwTriggerMask);
    }
}

void CUnitGroup::damagedBot( float fDamage, CUnit* pSource, uint32_t dwTriggerMask )
{
    CGameUnit* pUnit;
    CCObject* pObj;
    CCARRAY_FOREACH(&m_oArrUnits, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->damagedBot(fDamage, pSource, dwTriggerMask);
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
    return !pUnit->isDead() && pUnit->isEnemyOf(pParam) && pUnit->getStatus() != CGameUnit::kNoAttacked;
}

CCUnitLayer::CCUnitLayer()
{
}

bool CCUnitLayer::init()
{
    m_oArrUnit.init();
    m_oArrProjectile.init();
    m_oUnitDustbin.init();
    m_oProjectileDustbin.init();
    m_fUnitTickInterval = 0;
    m_iPendingSkillOwner = 0;
    return CCLayerColor::init();
}

bool CCUnitLayer::initWithColor( const ccColor4B& color )
{
    m_oArrUnit.init();
    m_oArrProjectile.init();
    m_oUnitDustbin.init();
    m_oProjectileDustbin.init();
    m_fUnitTickInterval = 0.1;
    m_iPendingSkillOwner = 0;
    return CCLayerColor::initWithColor(color);
}

void CCUnitLayer::onEnter()
{
    CCLayerColor::onEnter();
    schedule(schedule_selector(CCUnitLayer::onTickEvent), m_fUnitTickInterval);
}

void CCUnitLayer::onExit()
{
    unschedule(schedule_selector(CCUnitLayer::onTickEvent));
    CCArray* pArrUnit = getUnits()->getUnitsArray();
    CGameUnit* pU;
    CCObject* pObj;
    CCARRAY_FOREACH(pArrUnit, pObj)
    {
        pU = dynamic_cast<CGameUnit*>(pObj);
        moveUnitToDustbin(pU);
    }
    clearUnitDustbin();

    CCArray* pArrProj = getProjectiles()->getUnitsArray();
    CProjectile* pP;
    CCARRAY_FOREACH(pArrProj, pObj)
    {
        pP = dynamic_cast<CProjectile*>(pObj);
        moveProjectileToDustbin(pP);
    }
    clearProjectileDustbin();
    CCLayerColor::onExit();
}

void CCUnitLayer::onTickEvent( float fDt )
{
    CCArray* pArrUnit = getUnits()->getUnitsArray();
    CCArray* pArrProj = getProjectiles()->getUnitsArray();
    CCArray oArr;

    CGameUnit* pUnit;
    CCObject* pObj = NULL;
    oArr.initWithArray(pArrUnit);
    CCARRAY_FOREACH(&oArr, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->onTick(fDt);
    }
    clearUnitDustbin();

    CProjectile* pProjectile;
    oArr.initWithArray(pArrProj);
    CCARRAY_FOREACH(&oArr, pObj)
    {
        pProjectile = dynamic_cast<CProjectile*>(pObj);
        pProjectile->onTick(fDt);
    }
    clearProjectileDustbin();
}

void CCUnitLayer::addUnit( CGameUnit* pUnit )
{
    m_oArrUnit.addUnit(pUnit);
    pUnit->setUnitLayer(this);
    addChild(pUnit->getSprite());
    addChild(pUnit->getShadowNode(), 10);
}

void CCUnitLayer::addProjectile( CProjectile* pProjectile )
{
    m_oArrProjectile.addUnit(pProjectile);
    pProjectile->setUnitLayer(this);
    addChild(pProjectile->getSprite());
}
/*

 void CCUnitLayer::delUnit( CGameUnit* pUnit )
 {
 M_DEF_GM(pGm);
 CProjectile* pProj = dynamic_cast<CProjectile*>(pUnit);
 if (pProj)
 {
 pGm->moveProjectileToDustbin(pProj);
 }
 else
 {
 pGm->moveUnitToDustbin(pUnit);
 }
 }*/

void CCUnitLayer::setUnitTickInterval( float fInterval )
{
    m_fUnitTickInterval = fInterval;
    schedule(schedule_selector(CCUnitLayer::onTickEvent), m_fUnitTickInterval);
}

float CCUnitLayer::getUnitTickInterval() const
{
    return m_fUnitTickInterval;
}

CUnitGroup* CCUnitLayer::getUnits()
{
    return &m_oArrUnit;
}

CGameUnit* CCUnitLayer::getUnitByKey( int iKey )
{
    return m_oArrUnit.getUnitByKey(iKey);
}

CUnitGroup* CCUnitLayer::getProjectiles()
{
    return &m_oArrProjectile;
}

CProjectile* CCUnitLayer::getProjectileByKey( int iKey )
{
    return dynamic_cast<CProjectile*>(m_oArrProjectile.getUnitByKey(iKey));
}

void CCUnitLayer::moveUnitToDustbin( CGameUnit* pToDel )
{
    m_oUnitDustbin.addObject(pToDel);
}

void CCUnitLayer::moveProjectileToDustbin( CProjectile* pToDel )
{
    m_oProjectileDustbin.addObject(pToDel);
}

CCArray* CCUnitLayer::getUnitDustbin()
{
    return &m_oUnitDustbin;
}

CCArray* CCUnitLayer::getProjectileDustbin()
{
    return &m_oProjectileDustbin;
}

void CCUnitLayer::clearUnitDustbin()
{
    CCObject* pObj;
    CGameUnit* pUnit;
    CCArray* pArrUnit = m_oArrUnit.getUnitsArray();
    CCARRAY_FOREACH(&m_oUnitDustbin, pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        pUnit->removeAllSpriteFromTheirParents();
        pArrUnit->removeObject(pUnit);
    }
    m_oUnitDustbin.removeAllObjects();
}

void CCUnitLayer::clearProjectileDustbin()
{
    CCObject* pObj;
    CProjectile* pProj;
    CCArray* pArrProj = m_oArrProjectile.getUnitsArray();
    CCARRAY_FOREACH(&m_oProjectileDustbin, pObj)
    {
        pProj = dynamic_cast<CProjectile*>(pObj);
        pProj->removeAllSpriteFromTheirParents();
        pArrProj->removeObject(pProj);
    }
    m_oProjectileDustbin.removeAllObjects();
}

int CCUnitLayer::touchActionIndex() const
{
    if (m_iPendingSkillOwner)
    {
        return kUnitCastTarget;
    }
    return kNormalTouch;
}

void CCUnitLayer::preOrderUnitToCast( int iUnit, int iSkill )
{
    m_iPendingSkillOwner = iUnit;
    CGameUnit* pUnit = getUnitByKey(m_iPendingSkillOwner);
    if (!pUnit || pUnit->isDead())
    {
        return;
    }
    CActiveSkill* pSkill = dynamic_cast<CActiveSkill*>(pUnit->getSkill(iSkill));
    if (!pSkill)
    {
        return;
    }
    pUnit->setToCastSkill(pSkill);
}

void CCUnitLayer::endOrderUnitToCast()
{
    CGameUnit* pUnit = getUnitByKey(m_iPendingSkillOwner);
    if (pUnit)
    {
        CCSkillButtonAdvance* pBtn = pUnit->getToCastSkill()->getSkillButton();
        if (pBtn->isPressed())
        {
            pBtn->setPressed(NULL);
        }
    }
    m_iPendingSkillOwner = 0;
}

void CCUnitLayer::orderUnitToCast( const CCPoint& roTargetPos )
{
    if (!m_iPendingSkillOwner)
    {
        return;
    }
    CGameUnit* pUnit = getUnitByKey(m_iPendingSkillOwner);
    if (!pUnit || pUnit->isDead())
    {
        m_iPendingSkillOwner = 0;
        return;
    }
    CActiveSkill* pSkill = pUnit->getToCastSkill();
    if (!pSkill)
    {
        m_iPendingSkillOwner = 0;
        return;
    }
    pSkill->setTargetPoint(roTargetPos);
    pUnit->cast();
    m_iPendingSkillOwner = 0;
}

void CCUnitLayer::orderUnitToCast( CGameUnit* pTargetUnit )
{
    if (!m_iPendingSkillOwner)
    {
        return;
    }
    CGameUnit* pUnit = getUnitByKey(m_iPendingSkillOwner);
    if (!pUnit || pUnit->isDead())
    {
        m_iPendingSkillOwner = 0;
        return;
    }
    CActiveSkill* pSkill = pUnit->getToCastSkill();
    if (!pSkill)
    {
        m_iPendingSkillOwner = 0;
        return;
    }
    pSkill->setTargetUnit(pTargetUnit->getKey());
    pSkill->setTargetPoint(pTargetUnit->getPosition());
    pUnit->cast();
    m_iPendingSkillOwner = 0;
}

void CCUnitLayer::onUnitDie( CGameUnit* pUnit )
{

}

const float CCWinUnitLayer::CONST_MIN_MOVE_DELTA = 10.0;
const float CCWinUnitLayer::CONST_MAX_CAN_MOVE_DURATION = 0.15;

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
    schedule(schedule_selector(CCWinUnitLayer::bufferWindowEffect), 1.0 / 200);
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

int CCWinUnitLayer::touchActionIndex() const
{
    if (isSlideAction())
    {
        return kSlideWindow;
    }
    else if (isClickAction())
    {
        int iIndex = CCUnitLayer::touchActionIndex();
        return iIndex == kNormalTouch ? kClickPoint : iIndex;
    }
    return CCUnitLayer::touchActionIndex();
}

CUnitInfo::CUnitInfo( const char* pName, const CCPoint& roAnchor, float fHalfOfWidth, float fHalfOfHeight, float fScale, const ARR_ANI_INFO_NAME& roArrAniInfoNames, const ARR_ANI_INFO_DELAY& roArrAniInfoDelays, const ARR_ANI_INFO_EFFECT& roArrAniInfoEffects, const ARR_ATTACK_ANI& roArrAttackAnis, float fBaseMoveSpeed, float fBaseAttackInterval, float fAttackMinRange, float fAttackRange, float fHostilityRange, CGameUnit::WEAPON_TYPE eWeaponType, int iProjectileKey, float fProjectileMoveSpeed, float fProjectileScale, float fProjectileMaxOffsetY, float fProjectileBirthOffsetX, float fProjectileBirthOffsetY, const CAttackValue& roBaseAttackValue, float fExAttackRandomRange, CArmorValue::ARMOR_TYPE eArmorType, float fBaseArmorValue, int iForceIndex, uint32_t dwForceAlly, float fMaxHp, bool bIsFixed, int iRewardGold, int iRewardExp )
: m_sName(pName)
, m_oAnchor(roAnchor)
, m_fHalfOfWidth(fHalfOfWidth)
, m_fHalfOfHeight(fHalfOfHeight)
, m_fScale(fScale)
, m_fBaseMoveSpeed(fBaseMoveSpeed)
, m_fBaseAttackInterval(fBaseAttackInterval)
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

    n = roArrAniInfoNames.count();
    typedef const char* PCSTR;
    const PCSTR* pAniInfoNames = roArrAniInfoNames;
    const double* pAniInfoDelays = roArrAniInfoDelays;
    const double* pAniInfoEffects = roArrAniInfoEffects;
    for (int i = 0; i < n; ++i)
    {
        CGameUnit::ANIMATION_INFO stAni(roArrAniInfoNames[i], roArrAniInfoDelays[i], roArrAniInfoEffects[i]);
        m_vecAniInfo.push_back(stAni);
    }
}

CUnitInfo::~CUnitInfo(void)
{
}

#if 0
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
#else
const uint16_t CUnitInfoPatch::CONST_FILE_DATA_SIZE
= sizeof(int)
+ sizeof(char[32])
+ sizeof(float)
+ sizeof(float)
+ sizeof(float)
+ sizeof(float)
+ sizeof(float)
+ sizeof(float)
+ sizeof(float)
+ sizeof(CAttackValue::ARR_ATTACK_VALUES)
+ sizeof(float)
+ sizeof(CArmorValue::ARMOR_TYPE)
+ sizeof(float)
+ sizeof(float)
+ sizeof(bool)
+ sizeof(int)
+ sizeof(int);
#endif

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
/*

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
 */

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

CProjectile* CProjectileManager::getProjectileByIndex( int iIndex )
{
    return dynamic_cast<CProjectile*>(m_oArrProjectile.getUnitByIndex(iIndex));
}

CProjectile* CProjectileManager::copyProjectile( int iKey )
{
    return dynamic_cast<CProjectile*>(m_oArrProjectile.getUnitByKey(iKey)->copy());
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
