#include "CommInc.h"

#include "GameDisplay.h"
#include "GameLogic.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Action.h"
#include "Unit.h"
#include "BattleGroundScene.h"
#include "Skill.h"
#include "GameCtrl.h"
#include "UnitInfo.h"

CSkill::CSkill()
    : m_iKey(CCGameManager::sharedGameManager()->getLogicBody()->keygen())
    , m_pOwner(NULL)
    , m_fCoolDown(0)
    , m_fCDLeft(0)
    , m_pDisplayBody(NULL)
{

}

CSkill::~CSkill()
{
    //CCLOG("~CSkill");
}

bool CSkill::init()
{
    m_fCoolDown = 0;
    m_fCDLeft = 0;
    return true;
}
/*
void CSkill::setName(const char* pName)
{
    m_sName = pName;
}

const char* CSkill::getName()
{
    return m_sName.c_str();
}
*/
void CSkill::onSkillAdd()
{
}

void CSkill::onSkillDel()
{
    CCSkillButtonBase* pDisplayBody = dynamic_cast<CCSkillButtonBase*>(getDisplayBody());
    if (!pDisplayBody)
    {
        return;
    }
    CCButtonPanel* pPanel = dynamic_cast<CCButtonPanel*>(pDisplayBody->getParent()->getParent());
    if (!pPanel)
    {
        return;
    }
    pPanel->delButton(pDisplayBody);
}

void CSkill::onSkillReady()
{
}

void CSkill::registerOnAttackTargetTrigger()
{
    m_pOwner->addToOnAttackTargetTrigger(this);
}

void CSkill::registerOnAttackedTrigger()
{
    m_pOwner->addToOnAttackedTrigger(this);
}

void CSkill::registerOnDamagedSurfaceTrigger()
{
    m_pOwner->addToOnDamagedSurfaceTrigger(this);
}

void CSkill::registerOnDamagedInnerTrigger()
{
    m_pOwner->addToOnDamagedInnerTrigger(this);
}

void CSkill::registerOnDamageTargetTrigger()
{
    m_pOwner->addToOnDamageTargetTrigger(this);
}

void CSkill::registerOnHpChangeTrigger()
{
    m_pOwner->addToOnHpChangeTrigger(this);
}

void CSkill::registerOnReviveTrigger()
{
    m_pOwner->addToOnReviveTrigger(this);
}

void CSkill::registerOnDieTrigger()
{
    m_pOwner->addToOnDieTrigger(this);
}

void CSkill::registerOnTickTrigger()
{
    m_pOwner->addToOnTickTrigger(this);
}

void CSkill::registerOnDestroyProjectileTrigger()
{
    m_pOwner->addToOnDestroyProjectileTrigger(this);
}

void CSkill::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
}

CAttackData* CSkill::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    return pAttack;
}

void CSkill::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
}

void CSkill::onUnitDamageTarget(float fDamage, CUnit* pTarget)
{
}

void CSkill::onUnitHpChange(float fChanged)
{
}

void CSkill::onUnitRevive()
{
}

void CSkill::onUnitDie()
{
}

void CSkill::onUnitTick(float fDt)
{
}

void CSkill::onUnitDestroyProjectile( CCProjectileWithAttackData* pProjectile )
{
}

void CSkill::unregisterOnAttackTargetTrigger()
{
    m_pOwner->delFromOnAttackTargetTrigger(this);
}

void CSkill::unregisterOnAttackedTrigger()
{
    m_pOwner->delFromOnAttackedTrigger(this);
}

void CSkill::unregisterOnDamagedSurfaceTrigger()
{
    m_pOwner->delFromOnDamagedSurfaceTrigger(this);
}

void CSkill::unregisterOnDamagedInnerTrigger()
{
    m_pOwner->delFromOnDamagedInnerTrigger(this);
}

void CSkill::unregisterOnDamageTargetTrigger()
{
    m_pOwner->delFromOnDamageTargetTrigger(this);
}

void CSkill::unregisterOnHpChangeTrigger()
{
    m_pOwner->delFromOnHpChangeTrigger(this);
}

void CSkill::unregisterOnReviveTrigger()
{
    m_pOwner->delFromOnReviveTrigger(this);
}

void CSkill::unregisterOnDieTrigger()
{
    m_pOwner->delFromOnDieTrigger(this);
}

void CSkill::unregisterOnDestroyProjectileTrigger()
{
    m_pOwner->delFromOnDestroyProjectileTrigger(this);
}

void CSkill::unregisterOnTickTrigger()
{
    m_pOwner->delFromOnTickTrigger(this);
}
/*
void CSkill::unregisterOnTickTriggerLater()
{
    m_pOwner->delFromOnTickTriggerLater(this);
}
*/
int CSkill::getKey() const
{
    return m_iKey;
}

CUnit* CSkill::getOwner() const
{
    return m_pOwner;
}

void CSkill::setOwner( CUnit* pOwner )
{
    m_pOwner = pOwner;
}
























CActiveSkill::CActiveSkill()
{
}

CActiveSkill::~CActiveSkill()
{
}

bool CActiveSkill::isCoolingDown() const
{
    return abs(m_fCDLeft) >= FLT_EPSILON;
}

void CActiveSkill::resetCD()
{
    m_fCDLeft = 0.0f;
    onSkillReady();
}

void CActiveSkill::onSkillCast()
{
}

bool CActiveSkill::init(float fCoolDown)
{
    m_fCoolDown = fCoolDown;
    m_fCDLeft = 0;
    m_iTargetUnit = 0;
    setTargetUnitHalfOfWidth(0.0);
    setCastTargetType(kNoTarget);
    setCastRange(0);
    setWeaponType(CGameUnit::kWTDelayed);
    setCastAniInfo(CGameUnit::kAnimationAct1, 0.0);

    return true;
}

void CActiveSkill::cast()
{
    CCAssert(canCast(), "call canCast() before this");

    CCSkillButtonAdvance* pBtn = dynamic_cast<CCSkillButtonAdvance*>(getDisplayBody());
    if (pBtn)
    {
        pBtn->coolDown();
    }
    m_fCDLeft = m_fCoolDown;
    onSkillCast();
}

bool CActiveSkill::canCast() const
{
    if (isCoolingDown())
    {
        return false;
    }
    return true;
}

void CActiveSkill::setTargetUnit(int var)
{
    CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
    CCAssert(u, "owner is null");
    CGameUnit* t = u->getUnitLayer()->getUnitByKey(var);
    if (!t)
    {
        m_iTargetUnit = 0;
        setTargetUnitHalfOfWidth(0.0);
        return;
    }
    m_iTargetUnit = var;
    setTargetUnitHalfOfWidth(t->getHalfOfWidth());
}

int CActiveSkill::getTargetUnit()
{
    return m_iTargetUnit;
}

const CCPoint& CActiveSkill::updateTargetUnitPoint()
{
    if (getCastTargetType() != kUnitTarget)
    {
        return getTargetPoint();
    }
    CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
    CCAssert(u, "owner is null");
    CGameUnit* t = u->getUnitLayer()->getUnitByKey(getTargetUnit());
    if (t)
    {
        setTargetPoint(t->getPosition());
    }
    
    return getTargetPoint();
}

CCSkillButtonAdvance* CActiveSkill::getSkillButton()
{
    return dynamic_cast<CCSkillButtonAdvance*>(getDisplayBody());
}

void CActiveSkill::setCastAniInfo( CGameUnit::ANIMATION_INDEX eAniIndex, float fCastEffectDelay )
{
    m_eAniIndex = eAniIndex;
    m_fCastEffectDelay = fCastEffectDelay;
}

CGameUnit::ANIMATION_INDEX CActiveSkill::getCastAniIndex() const
{
    return m_eAniIndex;
}

float CActiveSkill::getCastEffectDelay() const
{
    return m_fCastEffectDelay;
}

// CLevelLimitSkill
CLevelLimitSkill::CLevelLimitSkill()
{

}

bool CLevelLimitSkill::canBeLearning(uint32_t dwLvl) const
{
    return (m_dwLvl < m_dwMaxLvl) && (dwLvl >= m_dwLvl * m_dwLvlStp + m_dwLvlLmt);
}

CLevelLimitSkill::~CLevelLimitSkill()
{

}

bool CLevelLimitSkill::init()
{
    return true;
}




CPassiveSkill::CPassiveSkill()
{

}

CPassiveSkill::~CPassiveSkill()
{

}

bool CPassiveSkill::init()
{
    return true;
}

CBuffSkill::CBuffSkill()
    : m_fDuration(0)
    , m_fPass(0)
    , m_bCanBePlural(false)
{

}

CBuffSkill::~CBuffSkill()
{

}

bool CBuffSkill::init( float fDuration, bool bCanBePlural /*= false*/, int iSrcKey /*= 0*/ )
{
    m_fDuration = fDuration;
    m_fPass = 0;
    m_bCanBePlural = bCanBePlural;
    m_iSrcKey = iSrcKey;
    return true;
}

void CBuffSkill::timeStep( float dt )
{
    m_fPass += dt;
}

void CBuffSkill::delBuffIfTimeout()
{
    if (m_fPass > m_fDuration)
    {
        delBuffFromOwner();
    }
}

void CBuffSkill::onSkillAdd()
{
    registerOnTickTrigger();
    onBuffAdd();
    CPassiveSkill::onSkillAdd();
}

void CBuffSkill::onSkillDel()
{
    //unregisterOnTickTriggerLater();
    unregisterOnTickTrigger();
    onBuffDel(false);
    CPassiveSkill::onSkillDel();
}

void CBuffSkill::onSkillCover()
{
    //unregisterOnTickTriggerLater();
    unregisterOnTickTrigger();
    onBuffDel(true);
    CPassiveSkill::onSkillDel();
}

void CBuffSkill::onUnitTick( float fDt )
{
    timeStep(fDt);
    delBuffIfTimeout();
}

void CBuffSkill::onBuffAdd()
{
}

void CBuffSkill::onBuffDel(bool bCover)
{
}

void CBuffSkill::setPlural( bool bPlural )
{
    m_bCanBePlural = bPlural;
}

bool CBuffSkill::canBePlural() const
{
    return m_bCanBePlural;
}

void CBuffSkill::delBuffFromOwner(bool bAfterTriggerLoop)
{
    m_pOwner->delBuff(this, bAfterTriggerLoop);
}

bool CAuraPas::init( float fRange, uint32_t dwTargetFlag, float fInterval, int iBuffTemplateKey, int iBuffLevel )
{
    m_fPass = 0;
    m_fRange = fRange;
    m_dwTargetFlag = dwTargetFlag;
    m_fInterval = fInterval;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;
    return true;
}

CCObject* CAuraPas::copyWithZone( CCZone* pZone )
{
    return CAuraPas::create(m_fRange, m_dwTargetFlag, m_fInterval, m_iBuffTemplateKey, m_iBuffLevel);
}

void CAuraPas::onSkillAdd()
{
    registerOnTickTrigger();
    CPassiveSkill::onSkillAdd();
}

void CAuraPas::onSkillDel()
{
    unregisterOnTickTrigger();
    CPassiveSkill::onSkillDel();
}

void CAuraPas::onUnitTick( float fDt )
{
    m_fPass += fDt;
    if (m_fPass < m_fInterval)
    {
        return;
    }
    m_fPass = 0;

    CGameUnit* pOwner = dynamic_cast<CGameUnit*>(getOwner());
    M_DEF_SM(pSm);
    M_DEF_GM(pGm);

    //int iCount = pGc->getTankCount();
    
    CBuffSkill* pTmpltBuff = dynamic_cast<CBuffSkill*>(pSm->getSkill(m_iBuffTemplateKey));
    CCAssert(pTmpltBuff, "template buff was not found");
    
    float fDuration;
    int iOrgBuffLevel = pTmpltBuff->getLevel();
    if (m_iBuffLevel != iOrgBuffLevel)
    {
        pTmpltBuff->setLevel(m_iBuffLevel);
        fDuration = MAX(CAuraPas::CONST_MIN_DURATION, pTmpltBuff->getDuration());
        pTmpltBuff->setLevel(iOrgBuffLevel);
    }
    else
    {
        fDuration = MAX(CAuraPas::CONST_MIN_DURATION, pTmpltBuff->getDuration());
    }
    
    CBuffSkill* pBuff = NULL;
    CBuffSkill* pOldBuff = NULL;
    CGameUnit* pLoopTank = NULL;
    CCUnitLayer* pLayer = dynamic_cast<CGameUnit*>(getOwner())->getUnitLayer();
    CCArray* pArr = pLayer->getUnits()->getUnitsArray();
    CCObject* pObj;
    CCARRAY_FOREACH(pArr, pObj)
    {
        pLoopTank = dynamic_cast<CGameUnit*>(pObj);
        // TODO: switch target TARGET_FLAG
        if (!(
            ((pLoopTank == pOwner) && (m_dwTargetFlag & CAuraPas::kSelf)) ||
            (pLoopTank != pOwner) && (pLoopTank->isAllyOf(pOwner) && (m_dwTargetFlag & CAuraPas::kAlly)) ||
            (pLoopTank->isEnemyOf(pOwner) && (m_dwTargetFlag & CAuraPas::kEnemy))
            ))
        {
            continue;
        }

        if (pOwner->getDistance(pLoopTank) > m_fRange)
        {
            continue;
        }

        if (!pTmpltBuff->canBePlural())
        {
            pOldBuff = pLoopTank->getBuffByType(pTmpltBuff->getTypeKey());
            if (pOldBuff)
            {
                pOldBuff->setPass(0);
                continue;
            }
        }

        pBuff = dynamic_cast<CBuffSkill*>(pSm->copySkill(pTmpltBuff));
        pBuff->setSrcKey(getOwner()->getKey());
        pBuff->setLevel(m_iBuffLevel);
        pBuff->setDuration(fDuration);
        pLoopTank->addBuff(pBuff, true);
        
    }	
}

bool CSelfBuffMakerAct::init( float fCoolDown, int iBuffTemplateKey, int iBuffLevel )
{
    CActiveSkill::init(fCoolDown);
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;

    return true;
}

CCObject* CSelfBuffMakerAct::copyWithZone( CCZone* pZone )
{
    return CSelfBuffMakerAct::create(m_fCoolDown, m_iBuffTemplateKey, m_iBuffLevel);
}

void CSelfBuffMakerAct::onSkillCast()
{
    M_DEF_SM(pSm);
    CBuffSkill* pBuff = dynamic_cast<CBuffSkill*>(pSm->copySkill(m_iBuffTemplateKey));
    pBuff->setSrcKey(getOwner()->getKey());
    CCAssert(pBuff, "is null");
    pBuff->setLevel(m_iBuffLevel);
    m_pOwner->addBuff(pBuff);
}

bool CAttackBuffMakerPas::init( int iProbability, int iBuffTemplateKey, int iBuffLevel, const CExtraCoeff& roCoeff )
{
    CPassiveSkill::init();
    m_iProbability = iProbability;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;
    m_oCoeff = roCoeff;
    return true;
}

CCObject* CAttackBuffMakerPas::copyWithZone( CCZone* pZone )
{
    return CAttackBuffMakerPas::create(m_iProbability, m_iBuffTemplateKey, m_iBuffLevel, m_oCoeff);
}

void CAttackBuffMakerPas::onSkillAdd()
{
    CPassiveSkill::onSkillAdd();
    registerOnAttackTargetTrigger();
}

void CAttackBuffMakerPas::onSkillDel()
{
    unregisterOnAttackTargetTrigger();
    CPassiveSkill::onSkillDel();
}

void CAttackBuffMakerPas::onUnitAttackTarget( CAttackData* pAttack, CUnit* pTarget )
{
    M_DEF_SM(pSm);
    if (M_RAND_HIT(m_iProbability))
    {
        for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
        {
            pAttack->setAttack((CAttackValue::ATTACK_TYPE)i, m_oCoeff.getValue(pAttack->getAttack((CAttackValue::ATTACK_TYPE)i)));
        }
        
        CBuffSkill* pBuff = dynamic_cast<CBuffSkill*>(pSm->copySkill(m_iBuffTemplateKey));
        if (pBuff && pBuff->getDuration())
        {
            pBuff->setSrcKey(getOwner()->getKey());
            pBuff->setLevel(m_iBuffLevel);
            pAttack->addBuff(pBuff, m_iProbability);
        }
        
    }
    
}

bool CHpChangeBuff::init(float fDuration, bool bCanBePlural, float fInterval, float fHpChange, bool bPercent, float fMinHp)
{
    CBuffSkill::init(fDuration, bCanBePlural);
    m_fInterval = fInterval;
    m_fIntervalPass = 0;
    m_fHpChange = fHpChange;
    m_bPercent = bPercent;
    m_fMinHp = fMinHp;

    return true;
}

CCObject* CHpChangeBuff::copyWithZone( CCZone* pZone )
{
    return CHpChangeBuff::create(m_fDuration, m_bCanBePlural, m_fInterval, m_fHpChange, m_bPercent, m_fMinHp);
}

void CHpChangeBuff::onUnitTick( float fDt )
{
    timeStep(fDt);

    if (m_fPass > m_fDuration)
    {
        m_fIntervalPass += fDt - m_fPass + m_fDuration;
    }
    else
    {
        m_fIntervalPass += fDt;
    }

    while (m_fIntervalPass >= m_fInterval)
    {
        onUnitInterval();
        m_fIntervalPass -= m_fInterval;
    }

    delBuffIfTimeout();
}

void CHpChangeBuff::onUnitInterval()
{
    float fNewHp = m_pOwner->getHp();
    if (m_bPercent)
    {
        fNewHp += m_pOwner->getMaxHp() * m_fHpChange / 100;
    }
    else
    {
        fNewHp += m_fHpChange;
    }

    if (fNewHp < m_fMinHp)
    {
        fNewHp = m_fMinHp;
    }
    m_pOwner->setHp(fNewHp);
}

bool CVampirePas::init(float fRatio)
{
    m_fRatio = fRatio;
    return true;
}

CCObject* CVampirePas::copyWithZone( CCZone* pZone )
{
    return CVampirePas::create(m_fRatio);
}

void CVampirePas::onSkillAdd()
{
    registerOnDamageTargetTrigger();
    CPassiveSkill::onSkillAdd();
}

void CVampirePas::onSkillDel()
{
    unregisterOnDamageTargetTrigger();
    CPassiveSkill::onSkillDel();
}

void CVampirePas::onUnitDamageTarget( float fDamage, CUnit* pTarget )
{
    m_pOwner->setHp(m_pOwner->getHp() + fDamage * m_fRatio);
}

bool CThumpPas::init( int iProbability, const CExtraCoeff& roCoeff, float fDuration )
{
    m_iProbability = iProbability;
    m_oCoeff = roCoeff;
    m_fDuration = fDuration;
    return true;
}

CCObject* CThumpPas::copyWithZone( CCZone* pZone )
{
    return CThumpPas::create(m_iProbability, m_oCoeff, m_fDuration);
}

void CThumpPas::onSkillAdd()
{
    registerOnAttackTargetTrigger();
    CPassiveSkill::onSkillAdd();
}

void CThumpPas::onSkillDel()
{
    registerOnAttackTargetTrigger();
    CPassiveSkill::onSkillDel();
}

void CThumpPas::onUnitAttackTarget( CAttackData* pAttack, CUnit* pTarget )
{
    if (M_RAND_HIT(m_iProbability))
    {
        for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
        {
            pAttack->setAttack((CAttackValue::ATTACK_TYPE)i, m_oCoeff.getValue(pAttack->getAttack((CAttackValue::ATTACK_TYPE)i)));
        }
        if (m_fDuration)
        {
            pAttack->addBuff(CStunBuff::create(m_fDuration, false), 100);
        }
    }
}

bool CStunBuff::init( float fDuration, bool bCanBePlural )
{
    CBuffSkill::init(fDuration, bCanBePlural);
    return true;
}

CCObject* CStunBuff::copyWithZone( CCZone* pZone )
{
    return CStunBuff::create(m_fDuration, m_bCanBePlural);
}

void CStunBuff::onBuffAdd()
{
    M_DEF_GM(pGm);
    pGm->playEffectSound("sound/cannon2.wav");
    CGameUnit* o = dynamic_cast<CGameUnit*>(getOwner());
    o->suspend();
}

void CStunBuff::onBuffDel(bool bCover)
{
    CGameUnit* o = dynamic_cast<CGameUnit*>(getOwner());
    o->resume();
}

bool CDoubleAttackPas::init( int iProbability )
{
    m_iProbability = iProbability;
    return true;
}

CCObject* CDoubleAttackPas::copyWithZone( CCZone* pZone )
{
    return CDoubleAttackPas::create(m_iProbability);
}

void CDoubleAttackPas::onSkillAdd()
{
    registerOnAttackTargetTrigger();
    CPassiveSkill::onSkillAdd();
}

void CDoubleAttackPas::onSkillDel()
{
    unregisterOnAttackTargetTrigger();
    CPassiveSkill::onSkillDel();
}

void CDoubleAttackPas::onUnitAttackTarget( CAttackData* pAttack, CUnit* pTarget )
{
    if (M_RAND_HIT(m_iProbability))
    {
        dynamic_cast<CGameUnit*>(getOwner())->setAttackCD(0);
    }
}

bool CHarmShieldBuff::init( float fDuration, bool bCanBePlural, const CExtraCoeff& roCoeff, bool bDelBuff )
{
    CBuffSkill::init(fDuration, bCanBePlural);
    m_oCoeff = roCoeff;
    m_bDelBuff = bDelBuff;
    return true;
}

CCObject* CHarmShieldBuff::copyWithZone( CCZone* pZone )
{
    return CHarmShieldBuff::create(m_fDuration, m_bCanBePlural, m_oCoeff, m_bDelBuff);
}

void CHarmShieldBuff::onBuffAdd()
{
    registerOnDamagedInnerTrigger();

    //CCGameManager* pGm = CCGameManager::sharedGameManager();
    //pGm->playEffectSound("sound/DispelMagicTarget.wav");

    CCCoverAct* pActCover = CCCoverAct::create(m_fDuration);
    dynamic_cast<CGameUnit*>(getOwner())->getShadowNode()->runAction(pActCover);
}

void CHarmShieldBuff::onBuffDel(bool bCover)
{
    unregisterOnDamagedInnerTrigger();
}

void CHarmShieldBuff::onUnitDamaged( CAttackData* pAttack, CUnit* pSource )
{
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; i++)
    {
        pAttack->setAttack((CAttackValue::ATTACK_TYPE)i, m_oCoeff.getValue(pAttack->getAttack((CAttackValue::ATTACK_TYPE)i)));
    }

    if (m_bDelBuff)
    {
        pAttack->m_oArrBuff.removeAllObjects();
    }
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName("bubble2.png");
    CCNode* pSn = dynamic_cast<CGameUnit*>(getOwner())->getShadowNode();
    pSn->runAction(CCFlash::create(0.0, pSprite, pAttack->getAngle()));
    //CCGameManager::sharedGameManager()->playEffectSound("sound/Fire_Energizer.wav");

}

bool CCommboPas::init(float fMaxInterval, const char* pFmt)
{
    m_fDt = 0;
    m_fMaxInterval = fMaxInterval;
    m_iCombo = 0;
    m_iComboOld = 0;
    m_sFmt = pFmt;
    return true;
}

CCObject* CCommboPas::copyWithZone( CCZone* pZone )
{
    return CCommboPas::create(m_fMaxInterval, m_sFmt.c_str());
}

void CCommboPas::onSkillAdd()
{
    registerOnDamageTargetTrigger();
    registerOnDamagedInnerTrigger();
    registerOnTickTrigger();
    m_pLabel = CCLabelTTF::create("          ", "Arial", 32);
    CCGameManager::sharedGameManager()->m_pCtrlLayer->addChild(m_pLabel, 1);
    CPassiveSkill::onSkillAdd();
}

void CCommboPas::onSkillDel()
{
    unregisterOnDamageTargetTrigger();
    unregisterOnDamagedInnerTrigger();
    unregisterOnTickTrigger();
    //m_pLabel->removeFromParentAndCleanup(true);
    CPassiveSkill::onSkillDel();
}

void CCommboPas::onUnitDamageTarget( float fDamage, CUnit* pTarget )
{
    m_fDt = 0;
    ++m_iCombo;
}

void CCommboPas::onUnitDamaged( CAttackData* pAttack, CUnit* pSource )
{
    //m_bHit = false;
    m_iCombo = 0;
}

void CCommboPas::onUnitTick( float fDt )
{
    if (m_iCombo > 0)
    {
        m_fDt += fDt;
    }
    
    if (m_fDt > m_fMaxInterval)
    {
        m_iCombo = 0;
    }
    else if (m_iCombo > 1 && m_iCombo != m_iComboOld)
    {
        m_iComboOld = m_iCombo;
        CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
        CCSize oSz2 = m_pLabel->getContentSize();
        char szTmp[128];
        sprintf(szTmp, m_sFmt.c_str(), m_iCombo);
        m_pLabel->setString(szTmp);
        int nG = MAX(0, 200 - 30 * m_iCombo);
        m_pLabel->runAction(CCJumpByEx::create(ccp(oSz.width - oSz2.width / 2, oSz.height * 4 / 5), ccp(-20, 50), ccc3(245, nG, 20)));
    }
    
}

bool CAttackTextPas::init(float fMaxInterval, const char* pFmt)
{
    m_fDt = 0;
    m_fMaxInterval = fMaxInterval;
    m_sFmt = pFmt;
    m_fValue = 0;
    m_pLabel = NULL;
    return true;
}

CCObject* CAttackTextPas::copyWithZone( CCZone* pZone )
{
    return CAttackTextPas::create(m_fMaxInterval, m_sFmt.c_str());
}

void CAttackTextPas::onSkillAdd()
{
    registerOnDamageTargetTrigger();
    registerOnDamagedInnerTrigger();
    registerOnTickTrigger();
    CPassiveSkill::onSkillAdd();
}

void CAttackTextPas::onSkillDel()
{
    unregisterOnDamageTargetTrigger();
    unregisterOnDamagedInnerTrigger();
    unregisterOnTickTrigger();
    //m_pLabel->removeFromParentAndCleanup(true);
    CPassiveSkill::onSkillDel();
}

void CAttackTextPas::onUnitDamageTarget( float fDamage, CUnit* pTarget )
{
    
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    CCShadowNode* pSn = dynamic_cast<CTank*>(getOwner())->getDisplayBody()->getShadowNode();
    if (!m_pLabel)
    {
        m_pLabel = CCLabelTTF::create("", "Arial", 16);
        pSn->addChild(m_pLabel, 1);
    }
    m_fValue += fDamage;
    m_fDt = 0;
    char szTmp[128]; 
    sprintf(szTmp, m_sFmt.c_str(), m_fValue);
    m_pLabel->setString(szTmp);
    m_pLabel->setFontSize(16 + MIN(m_fValue, 200) * 24 / 200.0);
    //int nG = MAX(0, 200 - 30 * m_iCombo);
    m_pLabel->runAction(CCJumpByEx::create(pSn->getAnchorPointInPoints(), ccp(0, 60), ccc3(0, 130, 200)));
}

void CAttackTextPas::onUnitDamaged( CAttackData* pAttack, CUnit* pSource )
{
    //m_bHit = false;
}

void CAttackTextPas::onUnitTick( float fDt )
{
    if (m_fValue > 0)
    {
        m_fDt += fDt;
    }

    if (m_fDt > m_fMaxInterval)
    {
        m_fValue = 0;
        m_fDt = 0;
    }
}

bool CScatterFireAct::init( float fCoolDown, float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY )
{
    CActiveSkill::init(fCoolDown);
    m_fPower = fPower;
    m_iCount = iCount;
    m_fDelta = fDelta;
    m_fRange = fRange;
    m_fVel = fVel;
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    return true;
}

CCObject* CScatterFireAct::copyWithZone( CCZone* pZone )
{
    return CScatterFireAct::create(m_fCoolDown, m_fPower, m_iCount, m_fDelta, m_fRange, m_fVel, m_fScaleX, m_fScaleY);
}

void CScatterFireAct::onSkillCast()
{
    CTank* pTankL = dynamic_cast<CTank*>(getOwner());
    CCTankSprite* pTank = dynamic_cast<CCTankSprite*>(pTankL->getDisplayBody());
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    
    CBullet* pBulletLogic;
    CCNormalBulletSprite* pBullet;
    CAttackData* pAtkData;

    float fBase = pTankL->getGunHeading() - (m_fDelta * m_iCount) / 2.0;

    for (int i = 0; i < m_iCount; i++)
    {
        pBulletLogic = CreateLogicObject<CBullet>();
        pBullet = CreateBulletSprite<CCNormalBulletSprite>("bullet/laser3.png", pBulletLogic, pTank);

        pBullet->setAnchorPoint(ccp(0, 0.5));
        pAtkData = CAttackData::create();
        pAtkData->setAttack(CAttackValue::kPhysical, m_fPower);

        pAtkData = getOwner()->attackAdv(pAtkData, NULL);
        pBullet->setAttackData(pAtkData);

        pBullet->setRotation(fBase + m_fDelta * i);

        pBullet->fire(m_fPower, m_fVel, m_fRange, m_fScaleX, m_fScaleY);
    }
    pGm->playEffectSound("sound/Fire_Disease.wav");
    
}

bool CShrapnelPas::init( float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY, int iProbability, int iMaxGeneration )
{
    m_fPower = fPower;
    m_iCount = iCount;
    m_fDelta = fDelta;
    m_fRange = fRange;
    m_fVel = fVel;
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_iProbability = iProbability;
    m_iMaxGeneration = iMaxGeneration;
    return true;
}

CCObject* CShrapnelPas::copyWithZone( CCZone* pZone )
{
    return CShrapnelPas::create(m_fPower, m_iCount, m_fDelta, m_fRange, m_fVel, m_fScaleX, m_fScaleY, m_iProbability, m_iMaxGeneration);
}

void CShrapnelPas::onSkillAdd()
{
    registerOnDestroyProjectileTrigger();
    CPassiveSkill::onSkillAdd();
}

void CShrapnelPas::onSkillDel()
{
    unregisterOnDestroyProjectileTrigger();
    CPassiveSkill::onSkillDel();
}

void CShrapnelPas::onUnitDestroyProjectile( CCProjectileWithAttackData* pProjectile )
{
    int iGeneration = pProjectile->getGeneration();
    if (iGeneration > m_iMaxGeneration || !M_RAND_HIT(m_iProbability))
    {
        return;
    }

    CTank* pTankL = dynamic_cast<CTank*>(getOwner());
    CCTankSprite* pTank = dynamic_cast<CCTankSprite*>(pTankL->getDisplayBody());
    CCGameManager* pGm = CCGameManager::sharedGameManager();

    CBullet* pBulletLogic;
    CCNormalBulletSprite* pBullet;
    CAttackData* pAtkData;

    CCNode* pNode = dynamic_cast<CCNode*>(pProjectile);
    CCAssert(pNode, "the Projectile must be a CCNode");
    if (!pNode)
    {
        return;
    }
    float fBase = pNode->getRotation() - (m_fDelta * m_iCount) / 2.0;

    for (int i = 0; i < m_iCount; i++)
    {
        pBulletLogic = CreateLogicObject<CBullet>();
        pBullet = CreateBulletSprite<CCNormalBulletSprite>("bullet/laser3.png", pBulletLogic, pTank);
        
        pBullet->setPosition(pNode->getPosition());
        pBullet->setAnchorPoint(ccp(0, 0.5));
        pAtkData = CAttackData::create();
        pAtkData->setAttack(CAttackValue::kPhysical, m_fPower);

        pAtkData = getOwner()->attackAdv(pAtkData, NULL);
        pBullet->setAttackData(pAtkData);
        pBullet->setGeneration(iGeneration + 1);
        pBullet->setRotation(fBase + m_fDelta * i);

        pBullet->fire(m_fPower, m_fVel, m_fRange, m_fScaleX, m_fScaleY);
    }
    pGm->playEffectSound("sound/explo0.wav");
    pGm->playEffectSound("sound/Fire_Disease.wav");
}

bool CHomingMissileAct::init( float fCoolDown, float fPower, float fInitDuration, int iBuffTemplateKey, int iBuffLevel )
{
    CActiveSkill::init(fCoolDown);
    m_fPower = fPower;
    m_fInitDuration = fInitDuration;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;
    return true;
}

CCObject* CHomingMissileAct::copyWithZone( CCZone* pZone )
{
    return CHomingMissileAct::create(m_fCoolDown, m_fPower, m_fInitDuration, m_iBuffTemplateKey, m_iBuffLevel);
}

void CHomingMissileAct::onSkillCast()
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCSkillManager* pSm = CCSkillManager::sharedSkillManager();
    CUnit* pOwner = getOwner();
    CCTankSprite* pTank = dynamic_cast<CCTankSprite*>(dynamic_cast<CTank*>(pOwner)->getDisplayBody());
    CCTankSprite* pTargetTank = NULL;
    
    int iCount = pGm->getTankCount();
    if (iCount < 2)
    {
        return;
    }
    CCTankSprite* pIt = NULL;
    for (int i = 0; i < iCount; i++)
    {
        pIt = pGm->getTankByIndex(i);
        if (pIt != pTank)
        {
            if (!pTargetTank)
            {
                pTargetTank = pIt;
            }
            else if (pIt->getLogicBody()->getDistance(pTank->getLogicBody()) < pTargetTank->getLogicBody()->getDistance(pTank->getLogicBody()))
            {
                pTargetTank = pIt;
            }

        }
    }
    if (!pTargetTank)
    {
        return;
    }

    int iTargetKey = pTargetTank->getKey();

    CBullet* pBulletLogic = CreateLogicObject<CBullet>();
    CCHomingMissileSprite* pBullet = CreateBulletSprite<CCHomingMissileSprite>("bullet/bullet2.png", pBulletLogic, pTank);

    CAttackData* pAtkData = CAttackData::create();
    pAtkData->setAttack(CAttackValue::kPhysical, m_fPower);

    CBuffSkill* pBuff = dynamic_cast<CHpChangeBuff*>(pSm->copySkill(m_iBuffTemplateKey));
    CCAssert(pBuff, "is null");
    pBuff->setLevel(m_iBuffLevel);
    pAtkData->addBuff(pBuff, 100);

    pAtkData = pOwner->attackAdv(pAtkData, NULL);
    pBullet->setAttackData(pAtkData);

    pBullet->fire(m_fPower, iTargetKey, m_fInitDuration);
    pGm->playEffectSound("sound/Fire_Missiles.wav");
}

bool CStatusShowPas::init()
{
    return true;
}

CCObject* CStatusShowPas::copyWithZone( CCZone* pZone )
{
    return CStatusShowPas::create();
}

void CStatusShowPas::onSkillAdd()
{
#if 0
    CCGameSprite* pUnit = dynamic_cast<CTank*>(getOwner())->getDisplayBody();
    CCShadowNode* pSn = pUnit->getShadowNode();
#else
    CCGameUnitSprite* pUnit = dynamic_cast<CGameUnit*>(getOwner())->getSprite();
    CCNode* pSn = pUnit->getShadowNode();//->getControler()->getShadowNode();
#endif
    CCSize oSz(pUnit->getControler()->getHalfOfWidth() * 3, pUnit->getControler()->getHalfOfHeight() * 3);
    m_oProgressBar.init(CCSizeMake(oSz.width * pUnit->getScale(), 6), CCSprite::createWithSpriteFrameName("healthbar_fill.png"), CCSprite::createWithSpriteFrameName("healthbar_border.png"), 1, 1, true);
    pSn->addChild(&m_oProgressBar, 1);
    //CCPoint op = pSn->getAnchorPoint();
    m_oProgressBar.setPosition(ccpAdd(pSn->getAnchorPointInPoints(), ccp(0, oSz.height * pUnit->getScale())));
    onUnitHpChange(0);

    registerOnHpChangeTrigger();
    CPassiveSkill::onSkillAdd();
}

void CStatusShowPas::onSkillDel()
{
    m_oProgressBar.removeFromParentAndCleanup(true);

    unregisterOnHpChangeTrigger();
    CPassiveSkill::onSkillDel();
}

void CStatusShowPas::onUnitHpChange( float fChanged )
{
    CUnit* pOwner = getOwner();
    float fPer = pOwner->getHp() * 100 / pOwner->getMaxHp();
    m_oProgressBar.setPercentage(fPer);
    m_oProgressBar.setFillColor(ccc3(MIN(255, (100.0 - fPer) * 2.56 / 0.5), MIN(255, 2.56 / 0.5  * fPer), 0));
}

bool CHpChangePas::init(float fInterval, float fHpChange, bool bPercent, float fMinHp)
{
    m_fInterval = fInterval;
    m_fIntervalPass = 0;
    m_fHpChange = fHpChange;
    m_bPercent = bPercent;
    m_fMinHp = fMinHp;

    return true;
}

CCObject* CHpChangePas::copyWithZone( CCZone* pZone )
{
    return CHpChangePas::create(m_fInterval, m_fHpChange, m_bPercent, m_fMinHp);
}

void CHpChangePas::onUnitTick( float fDt )
{
    m_fIntervalPass += fDt;

    while (m_fIntervalPass >= m_fInterval)
    {
        onUnitInterval();
        m_fIntervalPass -= m_fInterval;
    }
}

void CHpChangePas::onUnitInterval()
{
    float fNewHp = m_pOwner->getHp();
    if (m_bPercent)
    {
        fNewHp += m_pOwner->getMaxHp() * m_fHpChange / 100;
    }
    else
    {
        fNewHp += m_fHpChange;
    }

    if (fNewHp < m_fMinHp)
    {
        fNewHp = m_fMinHp;
    }
    m_pOwner->setHp(fNewHp);
}

void CHpChangePas::onSkillAdd()
{
    registerOnTickTrigger();
    CPassiveSkill::onSkillAdd();
}

void CHpChangePas::onSkillDel()
{
    unregisterOnTickTrigger();
    CPassiveSkill::onSkillDel();
}

CSkillInfo::CSkillInfo()
    : m_iKey(CCGameManager::sharedGameManager()->getLogicBody()->keygen())
    , m_ppValue(NULL)
{
}

CSkillInfo::~CSkillInfo()
{
    if (m_ppValue)
    {
        for (int i = 0; i < m_iMaxLevel; i++)
        {
            delete[] m_ppValue[i];
        }
        delete[] m_ppValue;
    }
}

bool CSkillInfo::init( int iMaxLevel, int iMaxValueCount, const char* pName, const char* pDescribe, const char* pBtnImgBaseName )
{
    m_iMaxLevel = iMaxLevel;
    m_iMaxValueCount = iMaxValueCount;
    m_sName = pName;
    m_sDescribe = pDescribe;
    m_sBtnImg = pBtnImgBaseName;
    
    m_ppValue = new S_VALUE*[m_iMaxLevel];
    for (int i = 0; i < m_iMaxLevel; i++)
    {
        m_ppValue[i] = new S_VALUE[m_iMaxValueCount];
    }

    return true;
}

CCObject* CSkillInfo::copyWithZone( CCZone* pZone )
{
    return CSkillInfo::create(m_iMaxLevel, m_iMaxValueCount, m_sName.c_str(), m_sDescribe.c_str(), m_sBtnImg.c_str());
}

int CSkillInfo::getKey() const
{
    return m_iKey;
}

void CSkillInfo::setValueAsFloat( int iLevel, int iIndex, float fValue )
{
    CCAssert(iLevel < m_iMaxLevel && iIndex < m_iMaxValueCount, "out of index");
    S_VALUE& val = m_ppValue[iLevel][iIndex];
    val.isFloat = true;
    val.f = fValue;
}

void CSkillInfo::setValueAsInteger( int iLevel, int iIndex, int iValue )
{
    CCAssert(iLevel < m_iMaxLevel && iIndex < m_iMaxValueCount, "out of index");
    S_VALUE& val = m_ppValue[iLevel][iIndex];
    val.isFloat = false;
    val.f = iValue;
}

float CSkillInfo::getValueAsFloat( int iLevel, int iIndex ) const
{
    CCAssert(iLevel < m_iMaxLevel && iIndex < m_iMaxValueCount, "out of index");
    S_VALUE& val = m_ppValue[iLevel][iIndex];
    CCAssert(val.isFloat, "not float");
    return val.f;
}

int CSkillInfo::getValueAsInteger( int iLevel, int iIndex ) const
{
    CCAssert(iLevel < m_iMaxLevel && iIndex < m_iMaxValueCount, "out of index");
    S_VALUE& val = m_ppValue[iLevel][iIndex];
    CCAssert(!val.isFloat, "not integer");
    return val.i;
}

void CSkillInfo::setName( const char* pName )
{
    m_sName = pName;
}

const char* CSkillInfo::getName() const
{
    return m_sName.c_str();
}

void CSkillInfo::setDescribe( const char* pDescribe )
{
    m_sDescribe = pDescribe;
}

const char* CSkillInfo::getDescribe() const
{
    return m_sDescribe.c_str();
}

void CSkillInfo::setButtonImage( const char* pBtnImgBaseName )
{
    m_sBtnImg = pBtnImgBaseName;
}

const char* CSkillInfo::getButtonImage() const
{
    return m_sBtnImg.c_str();
}

bool CSpeedBuff::init( float fDuration, bool bCanBePlural, const CExtraCoeff& roExMoveSpeed, const CExtraCoeff& roExAttackSpeed )
{
    m_oExMoveSpeed = roExMoveSpeed;
    m_oExAttackSpeed = roExAttackSpeed;
    return CBuffSkill::init(fDuration, bCanBePlural);
}

CCObject* CSpeedBuff::copyWithZone( CCZone* pZone )
{
    return CSpeedBuff::create(m_fDuration, m_bCanBePlural, m_oExMoveSpeed, m_oExAttackSpeed);
}

void CSpeedBuff::onBuffAdd()
{
    CGameUnit* pUnit = dynamic_cast<CGameUnit*>(getOwner());
    CExtraCoeff oEx = pUnit->getExMoveSpeed();
    pUnit->setExMoveSpeed(CExtraCoeff(oEx.getMulriple() + m_oExMoveSpeed.getMulriple(), oEx.getAddend() + m_oExMoveSpeed.getAddend()));
    oEx = pUnit->getExAttackSpeed();
    pUnit->setExAttackSpeed(CExtraCoeff(oEx.getMulriple() + m_oExAttackSpeed.getMulriple(), oEx.getAddend() + m_oExAttackSpeed.getAddend()));
}

void CSpeedBuff::onBuffDel(bool bCover)
{
    CGameUnit* pUnit = dynamic_cast<CGameUnit*>(getOwner());
    CExtraCoeff oEx = pUnit->getExMoveSpeed();
    pUnit->setExMoveSpeed(CExtraCoeff(oEx.getMulriple() - m_oExMoveSpeed.getMulriple(), oEx.getAddend() - m_oExMoveSpeed.getAddend()));
    oEx = pUnit->getExAttackSpeed();
    pUnit->setExAttackSpeed(CExtraCoeff(oEx.getMulriple() - m_oExAttackSpeed.getMulriple(), oEx.getAddend() - m_oExAttackSpeed.getAddend()));
}

bool CSplashPas::init( float fNearRange, const CExtraCoeff& roExNearDamage, float fMidRange, const CExtraCoeff& roExMidDamage, float fFarRange, const CExtraCoeff& roExFarDamage )
{
    CPassiveSkill::init();
    m_fNearRange = fNearRange;
    m_oExNearDamage = roExNearDamage;
    m_fMidRange = fMidRange;
    m_oExMidDamage = roExMidDamage;
    m_fFarRange = fFarRange;
    m_oExFarDamage = roExFarDamage;
    return true;
}

CCObject* CSplashPas::copyWithZone( CCZone* pZone )
{
    return CSplashPas::create(m_fNearRange, m_oExNearDamage, m_fMidRange, m_oExMidDamage, m_fFarRange, m_oExFarDamage);
}

void CSplashPas::onSkillAdd()
{
    registerOnDamageTargetTrigger();
}

void CSplashPas::onSkillDel()
{
    unregisterOnDamageTargetTrigger();
}

void CSplashPas::onUnitDamageTarget( float fDamage, CUnit* pTarget )
{
    M_DEF_GM(pGm);
    CGameUnit* pT = dynamic_cast<CGameUnit*>(pTarget);
    CGameUnit* pO = dynamic_cast<CGameUnit*>(getOwner());
    if (!pT || !pO)
    {
        return;
    }
    uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;
    float fDis;
    CGameUnit* pUnit;
    CCObject* pObj;
    CCUnitLayer* pLayer = dynamic_cast<CGameUnit*>(getOwner())->getUnitLayer();
    CCARRAY_FOREACH(pLayer->getUnits()->getUnitsArray(), pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (!pUnit)
        {
            continue;
        }
        fDis = MAX(0, pUnit->getDistance(pT) - pUnit->getHalfOfWidth());
        if (fDis <= m_fFarRange && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pO)) && pUnit != pT)
        {
            if (fDis <= m_fNearRange)
            {
                pUnit->damagedBot(m_oExNearDamage.getValue(fDamage), pO, dwTriggerMask);
            }
            else if (fDis <= m_fMidRange)
            {
                pUnit->damagedBot(m_oExMidDamage.getValue(fDamage), pO, dwTriggerMask);
            }
            else
            {
                pUnit->damagedBot(m_oExFarDamage.getValue(fDamage), pO, dwTriggerMask);
            }
        }

    }
}

bool CSplashAct::init( float fCoolDown, float fNearRange, const CAttackValue& roNearDamage, float fMidRange, const CAttackValue& roMidDamage, float fFarRange, const CAttackValue& roFarDamage, int iBuffTemplateKey, int iBuffLevel )
{
    CActiveSkill::init(fCoolDown);
    m_fNearRange = fNearRange;
    m_oNearDamage = roNearDamage;
    m_fMidRange = fMidRange;
    m_oMidDamage = roMidDamage;
    m_fFarRange = fFarRange;
    m_oFarDamage = roFarDamage;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = m_iBuffLevel;
    return true;
}

CCObject* CSplashAct::copyWithZone( CCZone* pZone )
{
    return CSplashAct::create(m_fCoolDown, m_fNearRange, m_oNearDamage, m_fMidRange, m_oMidDamage, m_fFarRange, m_oFarDamage, m_iBuffTemplateKey, m_iBuffLevel);
}

void CSplashAct::onSkillAdd()
{
    CActiveSkill::onSkillAdd();
}

void CSplashAct::onSkillDel()
{
    CActiveSkill::onSkillDel();
}

void CSplashAct::onSkillCast()
{
    M_DEF_GM(pGm);
    M_DEF_SM(pSm);
    CGameUnit* pO = dynamic_cast<CGameUnit*>(getOwner());
    if (!pO)
    {
        return;
    }
    CAttackData* pAd;
    float fDis;
    CGameUnit* pUnit;
    CCObject* pObj;
    CBuffSkill* pTBuff = NULL;
    CBuffSkill* pBuff;
    uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;

    CCARRAY_FOREACH(pO->getUnitLayer()->getUnits()->getUnitsArray(), pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (!pUnit)
        {
            continue;
        }
        fDis = MAX(0, pUnit->getDistance(pO) - pUnit->getHalfOfWidth());
        if (fDis <= m_fFarRange && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pO)))
        {
            pAd = CAttackData::create();
            (!pTBuff) && (pTBuff = dynamic_cast<CBuffSkill*>(pSm->getSkill(m_iBuffTemplateKey)));
            pBuff = dynamic_cast<CBuffSkill*>(pTBuff->copy());
            pAd->addBuff(pBuff, 100);
            if (fDis <= m_fNearRange)
            {
                pAd->setAttack(m_oNearDamage);
                pUnit->damagedAdv(pAd, pO, dwTriggerMask);
            }
            else if (fDis <= m_fMidRange)
            {
                pAd->setAttack(m_oMidDamage);
                pUnit->damagedAdv(pAd, pO, dwTriggerMask);
            }
            else
            {
                pAd->setAttack(m_oFarDamage);
                pUnit->damagedAdv(pAd, pO, dwTriggerMask);
            }
        }
    }

    pO->getUnitLayer()->runAction(CCShakeAct::create(0.2, 0.02, 5));
}

CThrowBuff::CThrowBuff()
    : m_iActRotateKey(CGameManager::sharedGameManager()->keygen())
    , m_iActThrowKey(CGameManager::sharedGameManager()->keygen())
{

}

bool CThrowBuff::init( float fDuration, bool bCanBePlural, int iSrcKey, float fThrowRange, float fThrowDuration, float fMaxHeight, const CAttackValue& roDamage, float fDamageRange, int iBuffTemplateKey, int iBuffLevel )
{
    setThrowRange(fThrowRange);
    setThrowDuration(fThrowDuration);
    setMaxHeight(fMaxHeight);
    m_oDamage = roDamage;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;
    m_fDamageRange = fDamageRange;
    return CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
}

CCObject* CThrowBuff::copyWithZone( CCZone* pZone )
{
    return CThrowBuff::create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fThrowRange, m_fThrowDuration, m_fMaxHeight, m_oDamage, m_fDamageRange, m_iBuffTemplateKey, m_iBuffLevel);
}

void CThrowBuff::onBuffAdd()
{
    CBuffSkill::onBuffAdd();

    CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
    if (u->isFixed() || u->getSprite()->getActionByTag(m_iActThrowKey))
    {
        return;
    }
    CGameUnit* pS = u->getUnitLayer()->getUnitByKey(m_iSrcKey);
    pS->attack(pS->isDoingOr(CGameUnit::kIntended) ? pS->getLastAttackTarget() : 0, pS->isDoingOr(CGameUnit::kIntended));
    if (!u || u->isDead())
    {
        return;
    }

    u->suspend();
    CCPoint oEndPos;
    if (m_oEndPos.equals(CCPointZero))
    {
        float fA = rand();
        float fR = rand() % (int)m_fThrowRange;
        oEndPos = ccpAdd(u->getPosition(), ccp(cos(fA) * fR, sin(-fA) * fR));
    }
    else
    {
        oEndPos = m_oEndPos;
    }
    
    CCAction* pAct = CCSequence::createWithTwoActions(
        CCJumpTo::create(m_fThrowDuration, oEndPos, m_fMaxHeight, 1),
        //CCMoveToEx::create(m_fThrowDuration, oEndPos, m_fMaxHeight, false),
        CCCallFuncN::create(this, callfuncN_selector(CThrowBuff::onThrowEnd))
        );
    pAct->setTag(m_iActThrowKey);
    u->getSprite()->runAction(pAct);

    pAct = CCRepeatForever::create(CCRotateBy::create(1, rand() % 3600 - 1800));
    pAct->setTag(m_iActRotateKey);
    u->getSprite()->runAction(pAct);
}

void CThrowBuff::onBuffDel(bool bCover)
{
    CBuffSkill::onBuffDel(bCover);
}

void CThrowBuff::onThrowEnd( CCNode* pNode )
{
    M_DEF_SM(pSm);
    CBuffSkill* pBuff = dynamic_cast<CBuffSkill*>(pSm->copySkill(m_iBuffTemplateKey));
    pBuff->setLevel(m_iBuffLevel);

    CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
    if (!u)
    {
        return;
    }

    u->getSprite()->stopActionByTag(m_iActRotateKey);
    u->getSprite()->setRotation(0);

    //u->setAnimation(CGameUnit::kAnimationDie, 1, 1, CGameUnit::kActDie);

    if (u->isDead())
    {
        return;
    }
    
    CGameUnit* pS = u->getUnitLayer()->getUnitByKey(m_iSrcKey);
    CAttackData* pAd = CAttackData::create();
    pAd->setAttack(m_oDamage);
    pAd->addBuff(pBuff, 50);
    u->damagedAdv(pAd, pS);
    u->getUnitLayer()->getUnits()->getUnitsInRange(u->getPosition(), m_fDamageRange, -1, CONDITION(CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(pS))->damagedAdv(pAd, pS);
    u->getUnitLayer()->runAction(CCShakeAct::create(0.3, 0.02, 5));
}

CTransmitBuff::CTransmitBuff()
{
}

bool CTransmitBuff::init(float fDuration, bool bCanBePlural, int iSrcKey, vector<CCPoint>& vecRandomPoint, float fFadeInDuration,  float fFadeOutDuration, int iBuffTemplateKey, int iBuffLevel)
{
	setFadeInDuration(fFadeInDuration);
	setFadeOutDuration(fFadeOutDuration);
	m_iBuffTemplateKey = iBuffTemplateKey;
	m_iBuffLevel = iBuffLevel;
	m_vecRandomPoint = vecRandomPoint;
	return CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);

}

CCObject* CTransmitBuff::copyWithZone( CCZone* pZone )
{
	return CTransmitBuff::create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_vecRandomPoint, m_fFadeInDuration, m_fFadeOutDuration, m_iBuffTemplateKey, m_iBuffLevel);
}

void CTransmitBuff::onBuffAdd()
{
	CBuffSkill::onBuffAdd();
	CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
	if (!u || u->isDead())
	{
		return;
	}
	u->suspend();
	u->setStatus(CGameUnit::kNoAttacked);
	CCAction* pAct = CCSequence::createWithTwoActions(
		CCFadeOut::create(m_fFadeOutDuration),
		CCCallFuncN::create(this, callfuncN_selector(CTransmitBuff::onTransmitBegin))
		);
	u->getSprite()->runAction(pAct);
}

void CTransmitBuff::onBuffDel(bool bCover)
{
	CBuffSkill::onBuffDel(bCover);
}

void CTransmitBuff::onTransmitBegin( CCNode* pNode )
{
	CCPoint oPoint;
	CCGameUnitSprite* pSprint = dynamic_cast<CCGameUnitSprite*>(pNode);
	CGameUnit* pUnit = pSprint->getControler();
	CCPoint oDestPoint;

	if (m_vecRandomPoint.size() > 0)
	{
		oDestPoint = m_vecRandomPoint[rand()%(m_vecRandomPoint.size() - 1)];

	}
	else
	{
		CUnitPath* pPath = pUnit->getMovePath();
		const CCPoint& lastPoint = pUnit->getLastMoveToTarget();
		unsigned int i = 0;
		for (; i < pPath->m_vecPoints.size(); i++)
		{
			if (pPath->m_vecPoints[i].equals(lastPoint))
				break;
		}
		int iDestIndex = rand()%i;
		oDestPoint = pPath->m_vecPoints[iDestIndex];
		pUnit->setPathCurPos(iDestIndex + 1);

	}

	oPoint.setPoint(oDestPoint.x, oDestPoint.y);
	if (oDestPoint.equals(CCPointZero))
	{
		oPoint.setPoint(rand()%800, rand()%480);

	}
	pNode->setPosition(oPoint);
	CCAction* pAct = CCSequence::createWithTwoActions(
		CCFadeIn::create(m_fFadeInDuration),
		CCCallFuncN::create(this, callfuncN_selector(CTransmitBuff::onTransmitEnd))
		);
	pNode->runAction(pAct);
}

void CTransmitBuff::onTransmitEnd( CCNode* pNode )
{
	CCGameUnitSprite* pSprite = dynamic_cast<CCGameUnitSprite*>(pNode);
	pSprite->getControler()->resume();
	pSprite->getControler()->setStatus(CGameUnit::kNormal);
}

bool CProjectileAct::init( float fCoolDown, float fCastRange, const CAttackValue& roDamage, CProjectile* pProj, int iBuffTemplateKey, int iBuffLevel )
{
    CActiveSkill::init(fCoolDown);
    setCastTargetType(kUnitTarget);
    setCastRange(fCastRange);
    m_oDamage = roDamage;
    m_pTemplateProjectile = pProj;
    m_iBuffTemplateKey = iBuffTemplateKey;
    m_iBuffLevel = iBuffLevel;
    setTemplateProjectile(pProj);
    setProjectileMoveSpeed(pProj->getBaseMoveSpeed());
    setProjectileScale(pProj->getSprite()->getScale());
    setProjectileMaxOffsetY(0.0);
    return true;
}

CCObject* CProjectileAct::copyWithZone( CCZone* pZone )
{
    CProjectileAct* pSkill = CProjectileAct::create(m_fCoolDown, m_fCastRange, m_oDamage, m_pTemplateProjectile, m_iBuffTemplateKey, m_iBuffLevel);
    pSkill->setCastTargetType(getCastTargetType());
    pSkill->setCastRange(getCastRange());
    pSkill->setWeaponType(getWeaponType());
    pSkill->setProjectileMoveSpeed(getProjectileMoveSpeed());
    pSkill->setProjectileScale(getProjectileScale());
    return pSkill;
}

void CProjectileAct::onSkillAdd()
{
    CActiveSkill::onSkillAdd();
}

void CProjectileAct::onSkillDel()
{
    CActiveSkill::onSkillDel();
}

void CProjectileAct::onSkillCast()
{
    M_DEF_SM(pSm);

    CAttackData* pAtk = CAttackData::create();
    pAtk->setAttack(m_oDamage);

    CBuffSkill* pBuff = dynamic_cast<CBuffSkill*>(pSm->copySkill(m_iBuffTemplateKey));
    if (pBuff && pBuff->getDuration())
    {
        pBuff->setSrcKey(getOwner()->getKey());
        pBuff->setLevel(m_iBuffLevel);
        pAtk->addBuff(pBuff, 100);
    }
    
    CGameUnit* u = dynamic_cast<CGameUnit*>(getOwner());
    const CCPoint& roPos2 = updateTargetUnitPoint();

    CGameUnit* t = dynamic_cast<CGameUnit*>(u->getUnitLayer()->getUnitByKey(getTargetUnit()));
    CProjectile* pProj;

    M_DEF_PM(pPm);
    pProj = dynamic_cast<CProjectile*>(pPm->getProjectileByIndex(COrgUnitInfo::kWave1)->copy());
    pProj->fireWave(u->getUnitLayer(), u, u, roPos2, pAtk, getProjectileScale(), getProjectileBirthOffset(), pProj->getBaseMoveSpeed());
    return;

    switch (getWeaponType())
    {
    case CGameUnit::kWTClosely:
    case CGameUnit::kWTInstant:
        if (!getTemplateProjectile())
        {
            t->damagedAdv(pAtk, u);
        }
        else
        {
            pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
            pProj->fireInstant(u->getUnitLayer(), u, u, t, pAtk, getProjectileScale(), getProjectileBirthOffset());
            /*
            u->getUnitLayer()->addProjectile(pProj);
            pProj->setProjectileBirthOffset(getProjectileBirthOffset());
            pProj->setAttackData(pAtk);
            pProj->setOwner(u->getKey());
            pProj->setStart(u->getKey());
            pProj->setTarget(getTargetUnit());
            pProj->getSprite()->setScale(getProjectileScale());
            pProj->setPosition(t->getPosition());
            pProj->onDie();
            */
        }
        break;

    case CGameUnit::kWTDelayed:
        pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
        pProj->fireFolow(u->getUnitLayer(), u, u, t, pAtk, getProjectileScale(), getProjectileBirthOffset(), getProjectileMaxOffsetY(), getProjectileMoveSpeed());
        /*
        u->getUnitLayer()->addProjectile(pProj);
        pProj->setAttackData(pAtk);
        pProj->setOwner(u->getKey());
        pProj->setStart(u->getKey());
        pProj->setTarget(getTargetUnit());
        pProj->setBaseMoveSpeed(getProjectileMoveSpeed());
        const CCPoint& roPos1 = pProj->getPosition();
        const CCPoint& roPos2 = t->getPosition();
        float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(roPos2, roPos1)));
        pProj->getSprite()->setScale(getProjectileScale());
        pProj->getSprite()->setRotation(fA);
        pProj->setProjectileBirthOffset(getProjectileBirthOffset());
        pProj->setPosition(ccpAdd(u->getPosition(), ccp(u->getSprite()->isFlipX() ? -pProj->getProjectileBirthOffset().x : pProj->getProjectileBirthOffset().x, pProj->getProjectileBirthOffset().y)));

        CGameUnit::UNIT_MOVE_PARAMS oMp;
        oMp.bIntended = false;
        oMp.bAutoFlipX = false;
        oMp.fMaxOffsetY = u->getProjectileMaxOffsetY();
        pProj->followTo(t->getKey(), oMp);
        */
        break;

    }
}

void CProjectileAct::setTemplateProjectile( CProjectile* pProjectile )
{
    m_pTemplateProjectile = pProjectile;
}

CProjectile* CProjectileAct::getTemplateProjectile()
{
    return m_pTemplateProjectile;
}

bool CChainLightingBuff::init( float fDuration, bool bCanBePlural, int iSrcKey, float fMaxCastRange, float fMaxJumpDistance, int iMaxJumpCount, const CAttackValue &roDamage)
{
    CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
    m_fMaxCastRange = fMaxCastRange;
    m_fMaxJumpDistance = fMaxJumpDistance;
    m_iMaxJumpCount = fMaxJumpDistance;
    m_oDamage = roDamage;
    return true;
    
}

CCObject* CChainLightingBuff::copyWithZone(CCZone *pZone)
{
    CChainLightingBuff* pBuff = create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fMaxCastRange, m_fMaxJumpDistance, m_iMaxJumpCount, m_oDamage);
    pBuff->setEffectedUnitKey(m_vecEffectedUnitKey);
    return pBuff;
}

void CChainLightingBuff::onBuffAdd()
{
	CBuffSkill::onBuffAdd();
    CGameUnit* pSource = dynamic_cast<CGameUnit*>(getOwner());
	if (!pSource || pSource->isDead())
	{
		return;
	}
    M_DEF_PM(pPm);
   
    CAttackData* pAd = NULL;
    CGameUnit* pUnit = NULL;
    CGameUnit* pTarget = NULL;
    CCObject* pObj = NULL;
    float fMinDis = 99999999;
    float fDis = 0.0;
    uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;
    
    
    CCARRAY_FOREACH(pSource->getUnitLayer()->getUnits()->getUnitsArray(), pObj)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObj);
        if (!pUnit)
        {
            continue;
        }
        bool bflag = false;
        for (vector<int>::size_type i = 0; i < m_vecEffectedUnitKey.size(); i++)
        {
            if(pUnit->getKey() == m_vecEffectedUnitKey[i])
            {
                bflag = true;
                break;
            }
                
        }
        if (bflag
            || pUnit->getKey() == pSource->getKey())
        {
            continue;
        }
        if ((fDis = ccpDistance(pUnit->getPosition(), pSource->getPosition())) < m_fMaxJumpDistance
            && fMinDis > fDis
            && CUnitGroup::isLivingAllyOf(pUnit, dynamic_cast<CUnitForce*>(pSource)))
        {
            pTarget = pUnit;
            fMinDis = fDis;
        }

    }
    
    if (pTarget == NULL)
    {
        return;
    }
    pAd = CAttackData::create();
    pAd->setAttack(m_oDamage);
    pUnit->damagedAdv(pAd, pSource, dwTriggerMask);
    
    CProjectile* pProj = pPm->getProjectileByIndex(COrgUnitInfo::kLightning2);
    pProj = dynamic_cast<CProjectile*>(pProj->copy());
    pTarget->getUnitLayer()->addProjectile(pProj);
    pProj->setProjectileBirthOffset(pSource->getProjectileBirthOffset());
    pProj->setAttackData(pAd);
    pProj->setOwner(pSource->getKey());
    pProj->setTarget(pTarget);
    pProj->getSprite()->setScale(1);
    pProj->setTargetObj(this);
    pProj->setTargetFun(callfuncO_selector(CChainLightingBuff::turnNext));
    //pProj->setPosition(pTarget->getPosition());
    pProj->onDie();
    
    if(m_vecEffectedUnitKey.size() <= 0)
    {
        m_vecEffectedUnitKey.push_back(pTarget->getKey());
        m_vecEffectedUnitKey.push_back(pSource->getKey());
    }
    else
    {
        m_vecEffectedUnitKey.push_back(pTarget->getKey());
    }
    m_pNextUnit = pTarget;

}
void CChainLightingBuff::setEffectedUnitKey(const vector<int> &vecEffectedUnitKey)
{
    m_vecEffectedUnitKey = vecEffectedUnitKey;
}

vector<int>& CChainLightingBuff::getEffectedUnitKey()
{
    return m_vecEffectedUnitKey;
}

void CChainLightingBuff::onBuffDel(bool bCover)
{
	CBuffSkill::onBuffDel(bCover);    
}

void CChainLightingBuff::turnNext(CCObject* pObj)
{
    CChainLightingBuff* pSkillBuff = dynamic_cast<CChainLightingBuff*>(this->copy());
    pSkillBuff->setEffectedUnitKey(m_vecEffectedUnitKey);
    if ((int)m_vecEffectedUnitKey.size() > m_iMaxJumpCount + 1)
    {
        return;
        
    }
    CAttackData* pAd = CAttackData::create();
    pAd->setAttack(m_oDamage);
    pAd->addBuff(pSkillBuff, 100);
    if(m_pNextUnit != NULL
       && !m_pNextUnit->isDead())
    {
        m_pNextUnit->addBuff(pSkillBuff);
    }

}

bool CSwordStormSkill::init(int iProbability, float fDuration, float fMaxDamageRange, const CAttackValue &roMaxDamage, const CExtraCoeff &roDamageCoef, char *pActName)
{
    CPassiveSkill::init();
    m_iProbability = iProbability;
    m_fDuration = fDuration;
    m_fMaxDamageRange = fMaxDamageRange;
    m_oMaxDamage = roMaxDamage;
    m_oDamageCoef = roDamageCoef;
    m_pActName = pActName;
    m_fDelayPerUnit = 0.1;
    m_iCountAnimLoop = 1;
    return true;
}
CCObject* CSwordStormSkill::copyWithZone(cocos2d::CCZone *pZone)
{
    CSwordStormSkill* pRet = CSwordStormSkill::create(m_iProbability, m_fDuration, m_fMaxDamageRange, m_oMaxDamage, m_oDamageCoef, m_pActName);
    pRet->setDelayPerUnit(getDelayPerUnit());
    pRet->setCountAnimLoop(getCountAnimLoop());
    return pRet;
}
void CSwordStormSkill::onSkillAdd()
{
    CPassiveSkill::onSkillAdd();
    registerOnDamageTargetTrigger();
    
}

void CSwordStormSkill::onSkillDel()
{
    CPassiveSkill::onSkillDel();
}

void CSwordStormSkill::onUnitDamageTarget(float fDamage, CUnit *pTarget)
{
    if(!M_RAND_HIT(m_iProbability))
    {
        return;
    }
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    if (!pOwn || pOwn->isDead())
    {
        return;
    }
    M_DEF_GM(pGm);
    
    CCAnimation* pAnim = pGm->getUnitAnimation(pOwn->getName(), m_pActName);
    pAnim->setDelayPerUnit(getDelayPerUnit());
    pAnim->setLoops(getCountAnimLoop());
    CCAnimate* pActAni = CCAnimate::create(pAnim);
    CCAction* pAction = CCSequence::createWithTwoActions(
        CCRepeat::create(CCSequence::createWithTwoActions(
            pActAni,
            CCCallFuncO::create(this, callfuncO_selector(CSwordStormSkill::onActEndPerAnim), pOwn)
        ), m_fDuration / (pAnim->getDuration() * getCountAnimLoop())),
        CCCallFuncN::create(this, callfuncN_selector(CSwordStormSkill::onActSpinEnd))
    );
    pOwn->stopAttack();
    pAction->setTag(CGameUnit::kActSpin);
    pOwn->startDoing(CGameUnit::kSpinning);
    pOwn->getSprite()->runAction(pAction);
    
}

void CSwordStormSkill::onActEndPerAnim(CCObject* pObj)
{
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(pObj);
    CCObject* pObjItem = NULL;
    CGameUnit* pUnit = NULL;
    float fDis = 0.0;
    
    CCARRAY_FOREACH(pOwn->getUnitLayer()->getUnits()->getUnitsArray(), pObjItem)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObjItem);
        if (!pUnit || pUnit->isDead())
        {
            continue;
        }
        if ((fDis = ccpDistance(pUnit->getPosition(), pOwn->getPosition()))< m_fMaxDamageRange
            && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pOwn)))
        {
            //给范围内的敌人受到持续伤害
            CAttackData* pAttack = CAttackData::create();
            pAttack->setAttack(m_oMaxDamage);
            pUnit->damagedAdv(pAttack,  pOwn, UNIT_TRIGGER_MASK(CUnit::kDamageTargetTrigger));
        }
        
    }

}

void CSwordStormSkill::onActSpinEnd( CCNode* pObj )
{
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    pOwn->stopSpin();
}

bool CChainBuff::init( float fDuration, bool bCanBePlural, int iSrcKey, float fRange, int iMaxTimes, const CAttackValue& roDamage, CProjectile* pProj )
{
    CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
    m_fRange = fRange;
    setMaxTimes(iMaxTimes);
    m_oDamage = roDamage;
    m_iStartUnit = iSrcKey;
    m_iEndUnit = 0;
    setTemplateProjectile(pProj);
    setProjectileMoveSpeed(pProj->getBaseMoveSpeed());
    setProjectileScale(pProj->getSprite()->getScale());
    setProjectileMaxOffsetY(0.0);
    setWeaponType(CGameUnit::kWTDelayed);
    return true;
}

CCObject* CChainBuff::copyWithZone( CCZone* pZone )
{
    CChainBuff* pSkill = CChainBuff::create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fRange, m_iMaxTimes, m_oDamage, m_pTemplateProjectile);
    pSkill->setWeaponType(getWeaponType());
    pSkill->setProjectileScale(getProjectileScale());
    pSkill->setProjectileBirthOffset(getProjectileBirthOffset());
    pSkill->setProjectileMaxOffsetY(getProjectileMaxOffsetY());
    pSkill->setProjectileMoveSpeed(getProjectileMoveSpeed());
    return pSkill;
}

void CChainBuff::onBuffAdd()
{
    CBuffSkill::onBuffAdd();
    CGameUnit* o = dynamic_cast<CGameUnit*>(getOwner());
    m_iStartUnit = o->getKey();
    setMaxTimes(getMaxTimes() - 1);
    m_mapDamaged[m_iStartUnit] = true;
    //CCLOG("addBuff add %d to map, left %d times", m_iStartUnit, getMaxTimes());
}

void CChainBuff::onBuffDel(bool bCover)
{
    if (bCover)
    {
        return;
    }
    if (getMaxTimes() <= 0)
    {
        return;
    }
    CGameUnit* o = dynamic_cast<CGameUnit*>(getOwner());
    if (o->isDead())
    {
        //return;
    }
    CCUnitLayer* l = o->getUnitLayer();
    CGameUnit* t = l->getUnits()->getNearestUnitInRange(o->getPosition(), m_fRange, CONDITION(CChainBuff::checkConditions), this);
    if (!t)
    {
        return;
    }
    M_DEF_SM(pSm);

    m_iStartUnit = o->getKey();
    m_iEndUnit = t->getKey();
    CCLOG("Ligh: %d -> %d", m_iStartUnit, m_iEndUnit);

    CGameUnit* s = dynamic_cast<CGameUnit*>(l->getUnitByKey(m_iSrcKey));

    CAttackData* pAtk = CAttackData::create();
    pAtk->setAttack(m_oDamage);

    CChainBuff* pBuff = dynamic_cast<CChainBuff*>(copy());
    pBuff->setMaxTimes(getMaxTimes());
    pBuff->m_mapDamaged = m_mapDamaged;
    
    if (pBuff && pBuff->getDuration())
    {
        pBuff->setSrcKey(getSrcKey());
        pBuff->setLevel(getLevel());
        pAtk->addBuff(pBuff, 100);
    }

    //const CCPoint& roPos2 = t->getPosition();

    CProjectile* pProj;
    switch (getWeaponType())
    {
    case CGameUnit::kWTClosely:
    case CGameUnit::kWTInstant:
        if (!getTemplateProjectile())
        {
            t->damagedAdv(pAtk, s);
        }
        else
        {
            pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
            pProj->fireInstant(l, s, o, t, pAtk, getProjectileScale(), getProjectileBirthOffset());
            /*
            l->addProjectile(pProj);
            pProj->setProjectileBirthOffset(ccp(0.0, o->getHalfOfHeight()));
            pProj->setAttackData(pAtk);
            pProj->setOwner(m_iSrcKey);
            pProj->setStart(m_iStartUnit);
            pProj->setTarget(m_iEndUnit);
            pProj->getSprite()->setScale(getProjectileScale());
            pProj->setPosition(t->getPosition());
            pProj->onDie();
            */
        }
        break;

    case CGameUnit::kWTDelayed:
        pProj = dynamic_cast<CProjectile*>(getTemplateProjectile()->copy());
        pProj->fireFolow(l, s, o, t, pAtk, getProjectileScale(), getProjectileBirthOffset(), getProjectileMaxOffsetY(), getProjectileMoveSpeed());
        /*
        l->addProjectile(pProj);
        pProj->setAttackData(pAtk);
        pProj->setOwner(m_iSrcKey);
        pProj->setStart(m_iStartUnit);
        pProj->setTarget(m_iEndUnit);
        pProj->setBaseMoveSpeed(getProjectileMoveSpeed());
        const CCPoint& roPos1 = pProj->getPosition();
        const CCPoint& roPos2 = t->getPosition();
        float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(roPos2, roPos1)));
        pProj->getSprite()->setScale(getProjectileScale());
        pProj->getSprite()->setRotation(fA);
        pProj->setProjectileBirthOffset(ccp(0.0, o->getHalfOfHeight()));
        pProj->setPosition(ccpAdd(o->getPosition(), ccp(o->getSprite()->isFlipX() ? -pProj->getProjectileBirthOffset().x : pProj->getProjectileBirthOffset().x, pProj->getProjectileBirthOffset().y)));

        CGameUnit::UNIT_MOVE_PARAMS oMp;
        oMp.bIntended = false;
        oMp.bAutoFlipX = false;
        oMp.fMaxOffsetY = o->getHalfOfHeight();//o->getProjectileMaxOffsetY();
        pProj->followTo(m_iEndUnit, oMp);
        */
        break;

    }
}

bool CChainBuff::checkConditions( CGameUnit* pUnit, CChainBuff* pBuff )
{
    if (!CUnitGroup::isLivingAllyOf(pUnit, dynamic_cast<CUnitForce*>(pBuff->getOwner())) || pUnit == pBuff->getOwner() || pBuff->m_mapDamaged[pUnit->getKey()])
    {
        return false;
    }

    return true;
}

void CChainBuff::setTemplateProjectile( CProjectile* pProjectile )
{
    m_pTemplateProjectile = pProjectile;
}

CProjectile* CChainBuff::getTemplateProjectile()
{
    return m_pTemplateProjectile;
}


CJumpChopSkill::CJumpChopSkill()
{
    m_pLastTargetUnit = NULL;
    setCountAnimLoop(1);
    setDelayPerUnit(0.2);
    setCountPerJump(1);
    setDurationPerJump(1.0);
    
}

bool CJumpChopSkill::init(int iProbability, float fMaxJumpRange, int iMaxJumpCount, const CAttackValue &roDamage, char *pActName)
{
    CPassiveSkill::init();
    m_iProbability = iProbability;
    m_fMaxJumpRange = fMaxJumpRange;
    m_iMaxJumpCount = iMaxJumpCount;
    m_oMaxDamage = roDamage;
    m_pActName = pActName;
    m_oAttackPosRegulate = ccp(40, 0);
    return true;
}
CCObject* CJumpChopSkill::copyWithZone(cocos2d::CCZone *pZone)
{
    CJumpChopSkill* pSkill = create(m_iProbability, m_fMaxJumpRange, m_iMaxJumpCount, m_oMaxDamage, m_pActName);
    pSkill->setAttackPosRegulate(m_oAttackPosRegulate);
    return pSkill;
}
void CJumpChopSkill::onSkillAdd()
{
    CPassiveSkill::onSkillAdd();
    registerOnDamageTargetTrigger();
}

void CJumpChopSkill::onSkillDel()
{
    CPassiveSkill::onSkillDel();
}

void CJumpChopSkill::onUnitDamageTarget(float fDamage, CUnit *pTarget)
{
    if(!M_RAND_HIT(m_iProbability))
    {
        return;
    }
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    if (!pOwn || pOwn->isDead())
    {
        return;
    }
    m_vecEffectedUnitKey.clear();
    if(m_pLastTargetUnit != NULL)
        m_vecEffectedUnitKey.push_back(m_pLastTargetUnit->getKey());
    m_vecEffectedUnitKey.push_back(pOwn->getKey());
    m_pLastTargetUnit = NULL;
    onJumpChopEnd(pOwn);
 
}

void CJumpChopSkill::onJumpChopEnd(cocos2d::CCObject *pObj)
{
    CGameUnit* pU = dynamic_cast<CGameUnit*>(pObj);
    if (m_pLastTargetUnit != NULL && !m_pLastTargetUnit->isDead())
    {
        uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;
        CAttackData* pAttackData = CAttackData::create();
        pAttackData->setAttack(m_oMaxDamage);
        m_pLastTargetUnit->damagedAdv(pAttackData, pU, dwTriggerMask);
        
    }
    if (m_iMaxJumpCount + 2 <= (int)m_vecEffectedUnitKey.size()
        || pU->getUnitLayer()->getUnits()->getUnitsArray()->count() <= (int)m_vecEffectedUnitKey.size())
    {
        return;
    }
    if (!pU || pU->isDead())
    {
        return;
    }
    
    CGameUnit* pUnit = NULL;
    CGameUnit* pTarget = NULL;
    CCObject* pObjItem = NULL;
    float fMinDis = 99999999;
    float fDis = 0.0;    
    
    CCARRAY_FOREACH(pU->getUnitLayer()->getUnits()->getUnitsArray(), pObjItem)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObjItem);
        if (!pUnit || pUnit->isDead())
        {
            continue;
        }
        bool bflag = false;
        for (vector<int>::size_type i = 0; i < m_vecEffectedUnitKey.size(); i++)
        {
            if(pUnit->getKey() == m_vecEffectedUnitKey[i])
            {
                bflag = true;
                break;
            }
            
        }
        if (bflag)
        {
            continue;
        }
        if ((fDis = ccpDistance(pUnit->getPosition(), pU->getPosition())) < m_fMaxJumpRange
            && fMinDis > fDis
            && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pU)))
        {
            pTarget = pUnit;
            fMinDis = fDis;
        }
    }
    
    if (pTarget == NULL || pTarget->isDead())
    {
        return;
    }

    m_vecEffectedUnitKey.push_back(pTarget->getKey());
    
    M_DEF_GM(pGm);
    CCAnimation* pAnim = pGm->getUnitAnimation(pU->getName(), m_pActName);
    pAnim->setDelayPerUnit(getDelayPerUnit());
    pAnim->setLoops(getCountAnimLoop());
    CCAnimate* pActAni = CCAnimate::create(pAnim);
    
    CCFiniteTimeAction* pCallO = CCCallFuncO::create(this, callfuncO_selector(CJumpChopSkill::onJumpChopEnd), pU);
    
    CCAction* pAction = CCSequence::create(pActAni, pCallO, NULL);
    
    pAction->setTag(CGameUnit::kActSpin);
    
    pU->startDoing(CGameUnit::kSpinning);
    pU->turnTo(pTarget->getPosition());
    pU->moveTo(pTarget->getPosition());
    pU->getSprite()->runAction(pAction);
    
    m_pLastTargetUnit = pTarget;
    CCLOG("target key is %d", pTarget->getKey());
}

CJumpChopBuff::CJumpChopBuff()
{
    m_pLastTargetUnit = NULL;
    setCountAnimLoop(1);
    setDelayPerUnit(0.03);
    setHalfWidth(30.0);
}

bool CJumpChopBuff::init(float fDuration, bool bCanBePlural, int iSrcKey, float fMaxJumpRange, int iMaxJumpCount, const CAttackValue &roDamage, char *pActName)
{
    CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
    m_fMaxJumpRange = fMaxJumpRange;
    m_iMaxJumpCount = iMaxJumpCount;
    m_oMaxDamage = roDamage;
    m_pActName = pActName;
   // m_oAttackPosRegulate = ccp(30, 10);
    return true;
}
CCObject* CJumpChopBuff::copyWithZone(cocos2d::CCZone *pZone)
{
    CJumpChopBuff* pBuff = create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fMaxJumpRange, m_iMaxJumpCount, m_oMaxDamage, m_pActName);
    //pBuff->setAttackPosRegulate(m_oAttackPosRegulate);
    pBuff->setHalfWidth(m_fHalfWidth);
    return pBuff;
}
void CJumpChopBuff::onBuffAdd()
{
    CBuffSkill::onBuffAdd();    
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    if (!pOwn || pOwn->isDead())
    {
        return;
    }
    pOwn->startDoing(CGameUnit::kSpinning);
    m_fLastHalfWidth = pOwn->getHalfOfWidth();
    onJumpChopEnd(pOwn);
}

void CJumpChopBuff::onBuffDel(bool bCover)
{
    CBuffSkill::onBuffDel(bCover);
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    pOwn->setHalfOfWidth(m_fLastHalfWidth);
    pOwn->stopSpin();
    pOwn->suspend();

}
void CJumpChopBuff::getAttackPoint(CUnit *pTarget, CCPoint& oPos)
{
    CGameUnit* pT = dynamic_cast<CGameUnit*>(pTarget);
    CGameUnit* pU = dynamic_cast<CGameUnit*>(getOwner());
    
    float fDis = pT->getHalfOfWidth() + pU->getHalfOfWidth() + (pU->getAttackMinRange() + pU->getAttackRange()) * 0.5;
    const CCPoint& roPos1 = pU->getPosition();
    const CCPoint& roPos2 = pT->getPosition();
    oPos.setPoint(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y);
}

void CJumpChopBuff::onJumpChopEnd(cocos2d::CCObject *pObj)
{
    CGameUnit* pU = dynamic_cast<CGameUnit*>(pObj);
    if (m_pLastTargetUnit != NULL && !m_pLastTargetUnit->isDead())
    {
        uint32_t dwTriggerMask = CUnit::kMaskActiveTrigger;
        CAttackData* pAttackData = CAttackData::create();
        pAttackData->setAttack(m_oMaxDamage);
        m_pLastTargetUnit->damagedAdv(pAttackData, pU, dwTriggerMask);
        
    }
    if (m_iMaxJumpCount + 1 <= (int)m_vecEffectedUnitKey.size()
        || pU->getUnitLayer()->getUnits()->getUnitsArray()->count() <= (int)m_vecEffectedUnitKey.size())
    {
        pU->stopSpin();
        pU->resume();
        pU->setHalfOfWidth(m_fLastHalfWidth);
        return;
    }
    if (!pU || pU->isDead())
    {
        return;
    }
    
    CGameUnit* pUnit = NULL;
    CGameUnit* pTarget = NULL;
    CCObject* pObjItem = NULL;
    float fMinDis = 99999999;
    float fDis = 0.0;
    
    CCARRAY_FOREACH(pU->getUnitLayer()->getUnits()->getUnitsArray(), pObjItem)
    {
        pUnit = dynamic_cast<CGameUnit*>(pObjItem);
        if (!pUnit || pUnit->isDead())
        {
            continue;
        }
        bool bflag = false;
        for (vector<int>::size_type i = 0; i < m_vecEffectedUnitKey.size(); i++)
        {
            if(pUnit->getKey() == m_vecEffectedUnitKey[i])
            {
                bflag = true;
                break;
            }
            
        }
        if (bflag)
        {
            continue;
        }
        if ((fDis = ccpDistance(pUnit->getPosition(), pU->getPosition())) < m_fMaxJumpRange
            && fMinDis > fDis
            && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pU)))
        {
            pTarget = pUnit;
            fMinDis = fDis;
        }
    }
    
    if (pTarget == NULL || pTarget->isDead())
    {
        pU->stopSpin();
        pU->resume();
        pU->setHalfOfWidth(m_fLastHalfWidth);
        return;
    }
    
    m_vecEffectedUnitKey.push_back(pTarget->getKey());
    
    M_DEF_GM(pGm);
    CCAnimation* pAnim = pGm->getUnitAnimation(pU->getName(), m_pActName);
    pAnim->setDelayPerUnit(getDelayPerUnit());
    pAnim->setLoops(getCountAnimLoop());
    CCAnimate* pActAni = CCAnimate::create(pAnim);
    
    CCFiniteTimeAction* pCallO = CCCallFuncO::create(this, callfuncO_selector(CJumpChopBuff::onJumpChopEnd), pU);
    
    CCAction* pAction = CCSequence::create(pActAni, pCallO, NULL);
    
    pAction->setTag(CGameUnit::kActSpin);
    
    CCPoint oTargetPoint;
    getAttackPoint(pTarget, oTargetPoint);
    
    pU->turnTo(pTarget->getPosition());
    pU->setPosition(oTargetPoint);
    pU->getSprite()->runAction(pAction);
    
    m_pLastTargetUnit = pTarget;
    CCLOG("target key is %d", pTarget->getKey());
}

bool CThunderBolt2Buff::init( float fDuration, bool bCanBePlural, int iSrcKey, float fInterval, float fRange, const CAttackValue& roDamage )
{
    CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
    m_fRange = fRange;
    m_oDamage = roDamage;
    m_fInterval = fInterval;
    m_fIntervalPass = 0;
    return true;
}

CCObject* CThunderBolt2Buff::copyWithZone( CCZone* pZone )
{
    return CThunderBolt2Buff::create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fInterval, m_fRange, m_oDamage);
}

void CThunderBolt2Buff::onBuffAdd()
{
    CBuffSkill::onBuffAdd();
}

void CThunderBolt2Buff::onBuffDel(bool bCover)
{
    CBuffSkill::onBuffDel(bCover);
}

void CThunderBolt2Buff::onUnitTick( float fDt )
{
    timeStep(fDt);

    if (m_fPass > m_fDuration)
    {
        m_fIntervalPass += fDt - m_fPass + m_fDuration;
    }
    else
    {
        m_fIntervalPass += fDt;
    }

    while (m_fIntervalPass >= m_fInterval)
    {
        onUnitInterval();
        m_fIntervalPass -= m_fInterval;
    }

    delBuffIfTimeout();
}

void CThunderBolt2Buff::onUnitInterval()
{
    float xPos = rand()%(int)m_fRange;
    float yPos = rand()%(int)m_fRange;

    M_DEF_UM(pUm);
    CGameUnit* t = pUm->unitByInfo(2);
    t->setRewardExp(0);
    CGameUnit* o = dynamic_cast<CGameUnit*>(getOwner());
    CGameUnit* s = dynamic_cast<CGameUnit*>(o->getUnitLayer()->getUnitByKey(m_iSrcKey));
    o->getUnitLayer()->addUnit(t);

    CCPoint targetPoint = ccp(o->getPosition().x-m_fRange/2+xPos,o->getPosition().y-m_fRange/2+yPos);

    t->setPosition(ccp(o->getPosition().x-m_fRange/2+xPos,o->getPosition().y-m_fRange/2+yPos));
    t->getSprite()->setVisible(false);
    //t->attack(o->getKey());
    CGameUnit* t2 = pUm->unitByInfo(2);
    t2->setRewardExp(0);
    o->getUnitLayer()->addUnit(t2);
    t2->setPosition(ccp(o->getPosition().x-m_fRange/2+xPos,o->getPosition().y-m_fRange/2+yPos));
    t2->getSprite()->setVisible(false);

    M_DEF_PM(pPm);
    CAttackData *pAd = CAttackData::create();
    pAd->setAttack(m_oDamage);

    CProjectile * pProj = dynamic_cast<CProjectile*>(pPm->getProjectileByIndex(COrgUnitInfo::kLightning3)->copy());
    pProj->fireInstant(o->getUnitLayer(), o, t, t2, pAd, 2, ccp(0, 800));
    /*
    o->getUnitLayer()->addProjectile(pProj);
    pProj->setProjectileBirthOffset(ccp(0, 800));
    pProj->getSprite()->setScaleY(2);

    //pProj->setAttackData(pAd);
    pProj->setOwner(m_iSrcKey);
    pProj->setStart(t2->getKey());
    pProj->setTarget(t);
    //pProj->getSprite()->setScale(3.0);
    pProj->setPosition(t->getPosition());
    pProj->onDie();
    */

    o->getUnitLayer()->getUnits()
        ->getUnitsInRange(targetPoint,100,-1,CONDITION(CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(s))->damagedAdv(pAd, s, CUnit::kMaskActiveTrigger);
    
    t->setHp(0);
    t2->setHp(0);
}

bool CSwordStormBuff::init(float fDuration, bool bCanBePlural, int iSrcKey, float fMaxDamageRange, const CAttackValue &roMaxDamage, const CExtraCoeff &roDamageCoef, char *pActName)
{
    CBuffSkill::init(fDuration, bCanBePlural, iSrcKey);
    m_fMaxDamageRange = fMaxDamageRange;
    m_oMaxDamage = roMaxDamage;
    m_oDamageCoef = roDamageCoef;
    m_pActName = pActName;
    m_fDelayPerUnit = 0.1;
    m_iCountAnimLoop = 1;
    return true;
}

CCObject* CSwordStormBuff::copyWithZone(cocos2d::CCZone *pZone)
{
    CSwordStormBuff* pRet = CSwordStormBuff::create(m_fDuration, m_bCanBePlural, m_iSrcKey, m_fMaxDamageRange, m_oMaxDamage, m_oDamageCoef, m_pActName);
    pRet->setDelayPerUnit(getDelayPerUnit());
    pRet->setCountAnimLoop(getCountAnimLoop());
    return pRet;
}
void CSwordStormBuff::onBuffAdd()
{
    CBuffSkill::onBuffAdd();
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    if (!pOwn || pOwn->isDead())
    {
        return;
    }
    M_DEF_GM(pGm);

    CCAnimation* pAnim = pGm->getUnitAnimation(pOwn->getName(), m_pActName);
    pAnim->setDelayPerUnit(getDelayPerUnit());
    pAnim->setLoops(getCountAnimLoop());
    CCAnimate* pActAni = CCAnimate::create(pAnim);
    CCAction* pAction = CCRepeatForever::create(CCSequence::createWithTwoActions(
        pActAni,
        CCCallFuncO::create(this, callfuncO_selector(CSwordStormBuff::onActEndPerAnim), pOwn)
    ));
    pOwn->stopAttack();
    pAction->setTag(CGameUnit::kActSpin);
    pOwn->startDoing(CGameUnit::kSpinning);
    pOwn->getSprite()->runAction(pAction);
}

void CSwordStormBuff::onBuffDel(bool bCover)
{
    CBuffSkill::onBuffDel(bCover);
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(getOwner());
    pOwn->stopSpin();
}

void CSwordStormBuff::onActEndPerAnim(CCObject* pObj)
{
    CGameUnit* pOwn = dynamic_cast<CGameUnit*>(pObj);
    CCObject* pObjItem = NULL;
    CGameUnit* pUnit = NULL;
    float fDis = 0.0;
    
    
    //给范围内的敌人受到持续伤害
    CAttackData* pAttack = CAttackData::create();
    pAttack->setAttack(m_oMaxDamage);
    
    //    pOwn->getUnitLayer()->getUnits()->getUnitsInRange(pOwn->getPosition(), m_fMaxDamageRange, -1, CONDITION(CUnitGroup::isLivingEnemyOf), //dynamic_cast<CUnitForce*>(pOwn))->damagedAdv(pAttack,  pOwn, UNIT_TRIGGER_MASK(CUnit::kDamageTargetTrigger));
    
    
    CCARRAY_FOREACH(pOwn->getUnitLayer()->getUnits()->getUnitsArray(), pObjItem)
    {
        
        pUnit = dynamic_cast<CGameUnit*>(pObjItem);
        if (!pUnit || pUnit->isDead())
        {
            continue;
        }
        if ((fDis = ccpDistance(pUnit->getPosition(), pOwn->getPosition()))< m_fMaxDamageRange
            && CUnitGroup::isLivingEnemyOf(pUnit, dynamic_cast<CUnitForce*>(pOwn)))
        {
            //给范围内的敌人受到持续伤害
            CAttackData* pAttack = CAttackData::create();
            pAttack->setAttack(m_oMaxDamage);
            pUnit->damagedAdv(pAttack,  pOwn, UNIT_TRIGGER_MASK(CUnit::kDamageTargetTrigger));
        }
    }
}