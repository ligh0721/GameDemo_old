#include "CommInc.h"

#include "GameDisplay.h"
#include "GameLogic.h"
#include "Action.h"
#include "BulletSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "TankSprite.h"


CCTankEventTrigger::CCTankEventTrigger()
{
}

CCTankEventTrigger::~CCTankEventTrigger()
{
}

void CCTankEventTrigger::triggerOnTick(float fDt)
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onTick(CCGameManager::sharedGameManager()->getLogicBody(), fDt);
}

void CCTankEventTrigger::triggerOnFire(CCBulletSprite* pBullet )
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onFire(CCGameManager::sharedGameManager()->getLogicBody(), dynamic_cast<CBulletInfo*>(pBullet->getLogicBody()));
}

void CCTankEventTrigger::triggerOnDamage(CCBulletSprite* pBullet )
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onDamage(CCGameManager::sharedGameManager()->getLogicBody(), dynamic_cast<CBulletInfo*>(pBullet->getLogicBody()));
}

void CCTankEventTrigger::triggerOnHitWall()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onHitWall(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnHitTank(CCTankSprite* pTank )
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onHitTank(CCGameManager::sharedGameManager()->getLogicBody(), dynamic_cast<CTankInfo*>(pTank->getLogicBody()));
}

void CCTankEventTrigger::triggerOnDeath()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onDeath(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnKill(CCTankSprite* pKiller, CCTankSprite* pDead )
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onKill(CCGameManager::sharedGameManager()->getLogicBody(), dynamic_cast<CTankInfo*>(pKiller->getLogicBody()), dynamic_cast<CTankInfo*>(pDead->getLogicBody()));
}

void CCTankEventTrigger::triggerOnInit()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onInit(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnDestroy()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onDestroy(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnGameInit()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onGameInit(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnGameOver()
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onGameOver(CCGameManager::sharedGameManager()->getLogicBody());
}

void CCTankEventTrigger::triggerOnJionGame(CCTankSprite* pTank )
{
    if (!isTriggerOn())
    {
        return;
    }
    getEventInterface()->onJionGame(CCGameManager::sharedGameManager()->getLogicBody(), dynamic_cast<CTankInfo*>(pTank->getLogicBody()));
}

CCTankSprite::CCTankSprite()
    : m_fTurningVelocity(0.0)
    , m_fTurningGunVelocity(0.0)
    , m_fFirePower(0.0)
    , m_fFireRange(0.0)
{
}

CCTankSprite::~CCTankSprite()
{
}

bool CCTankSprite::initWithSpriteFrameNameAndLogicGun( const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite )
{
    initWithSpriteFrameName(pszSpriteFrameName);
    setGun(pGunSprite);
    setLogicBody(pTank);
    dynamic_cast<CDisplayInfoInterface*>(getLogicBody())->setDisplayBody(this);

    return true;
}

void CCTankSprite::update(float dt)
{
    triggerOnTick(dt);
    collisionDetection();
}

bool CCTankSprite::collisionDetection()
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    int iCount = pGm->getTankCount();
    for (int i = 0; i < iCount; i++)
    {
        CCTankSprite* pOtherTank = pGm->getTankByIndex(i);
        if (pOtherTank == this || pOtherTank->isExploding())
        {
            continue;
        }
        CCPoint oLoc = getPosition();
        CCPoint oPos = pOtherTank->getPosition();
        float fDis = ccpDistance(oLoc, oPos);
        float fMinDix = pOtherTank->getContentSize().width / 2 + getContentSize().width / 2;
        float fDelta = ccpToAngle(ccpSub(oPos, oLoc));
        if (fDis < fMinDix)
        //if (boundingBox().intersectsRect(pOtherTank->boundingBox()))
        {
            //getGameManager()->playEffectSound("sound/hitrobot.wav");
            fDis = (fMinDix - fDis) * 2;
            CCPoint oD = ccp(fDis * cos(fDelta), fDis * sin(fDelta));
            setPosition(ccpSub(oLoc, oD));
            pOtherTank->setPosition(ccpAdd(oPos, oD));
            stopMove();
            dynamic_cast<CCTankSprite*>(pOtherTank)->stopMove();
            return true;
        }
    }
    return false;
}

CEventInterface* CCTankSprite::getEventInterface() const
{
    return dynamic_cast<CEventInterface*>(getLogicBody());
}

bool CCTankSprite::isTriggerOn() const
{
    return !isExploding();
}

int CCTankSprite::getKey() const
{
    return getLogicBody()->getLogicKey();
}

void CCTankSprite::setLineVelocity( float fVel )
{
    if (isMoving())
    {
        return;
    }
    CCGameSprite::setLineVelocity(fVel);
}

bool CCTankSprite::isExploding() const
{
    return isDoingOr(kExploding);
}

bool CCTankSprite::isTurning() const
{
    return isDoingOr(kTurning);
}

bool CCTankSprite::isMoving() const
{
    return isDoingOr(kMoving);
}

bool CCTankSprite::isTurningGun() const
{
    return isDoingOr(kTurningGun);
}

bool CCTankSprite::isSuspended() const
{
    return isDoingOr(kSuspended);
}

CCNormalTankSprite::CCNormalTankSprite()
{
}

CCNormalTankSprite::~CCNormalTankSprite()
{
}

IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_GUN(CCNormalTankSprite);

void CCNormalTankSprite::update( float dt )
{
    if (isExploding())
    {
        return;
    }

    CCTankSprite::update(dt);

}

CCSprite* CCNormalTankSprite::getGun()
{
    return dynamic_cast<CCSprite*>(getChildByTag(0));
}

void CCNormalTankSprite::setGun( CCSprite* pGunSprite )
{
    CCSprite* pGun = dynamic_cast<CCSprite*>(getChildByTag(0));
    if (pGunSprite && (!pGun || pGun != pGunSprite))
    {
        if (pGun)
        {
            pGun->removeFromParentAndCleanup(true);
        }
        addChild(pGunSprite, 0, 0);
        pGunSprite->setPosition(getAnchorPointInPoints());
    }
}

void CCNormalTankSprite::explode()
{
    if (isExploding())
    {
        return;
    }

    CCGameManager::sharedGameManager()->playEffectSound("sound/explosion.wav");

    getChildByTag(0)->setVisible(false);

    CCFiniteTimeAction* pActExplode0 = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("explode"));
    CCFiniteTimeAction* pActExplode1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actExplodeEnd));
    CCAction* pActExplode = CCSequence::create(pActExplode0, pActExplode1, NULL);

    stop();
    startDoing(kExploding);
    this->runAction(pActExplode);
}

void CCNormalTankSprite::stop()
{
    if (isExploding() || isSuspended())
    {
        return;
    }
    //endDoing(kMoving | kTurningGun);
    m_dwDoingFlags = 0;
    getGun()->stopAllActions();
    stopAllActions();
}

void CCNormalTankSprite::fire( float fPower )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    CCGameManager* pGm = CCGameManager::sharedGameManager();
    pGm->playEffectSound("sound/Fire_Artillery.wav");

    setFirePower(fPower);

    CBullet* pBulletLogic = CreateLogicObject<CBullet>();
    CCNormalBulletSprite* pBullet = CreateBulletSprite<CCNormalBulletSprite>("bullet/bullet1.png", pBulletLogic, this);

    CAttackData* pAtkData = CAttackData::create();
    pAtkData->setAttack(CAttackValue::kPhysical, getFirePower());

    pAtkData = dynamic_cast<CUnit*>(getLogicBody())->attackAdv(pAtkData, NULL);
    pBullet->setAttackData(pAtkData);

    float fScale = MAX(0.3, MIN(2, fPower / 10));
    pBullet->fire(fPower, MAX(300, MIN(800, 800 - fPower * 50)), getFireRange(), fScale, fScale);

    int iCount = CCGameManager::sharedGameManager()->getTankCount();
    for (int i = 0; i < iCount; i++)
    {
        CCTankSprite* pOtherTank = pGm->getTankByIndex(i);
        if (pOtherTank != this)
        {
            pOtherTank->triggerOnFire(pBullet);
        }
    }
}

void CCNormalTankSprite::moveTo( const CCPoint& oPos, float fVel )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    endDoing(kMoving);

    if (fVel < 0)
    {
        setLineVelocity(-fVel);
    }

    float fVelTurn = getTurningVelocity();
    if (fVelTurn < 0)
    {
        setTurningVelocity(-fVelTurn);
    }

    CCPoint oLoc = getPosition();
    float fA = -ccpToAngle(ccpSub(oPos, oLoc));
    float fR = getRotation();
    float fDelta = CCGameSprite::reviseRotation(CC_RADIANS_TO_DEGREES(fA) - fR);

    float fDist = ccpDistance(oLoc, oPos);

    CCFiniteTimeAction* pActMove0 = CCRotateBy::create(abs(fDelta / fVelTurn), fDelta);
    CCFiniteTimeAction* pActMove1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actTurnEnd));
    CCFiniteTimeAction* pActMove2 = CCMoveTo::create(abs(fDist / fVel), oPos);
    CCFiniteTimeAction* pActMove3 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actMoveEnd));
    CCAction* pActMove = CCSequence::create(pActMove0, pActMove1, pActMove2, pActMove3, NULL);

    stopAllActions();
    startDoing(kTurning | kMoving);
    runAction(pActMove);
}

void CCNormalTankSprite::moveBy( float fDistance, float fVel )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    endDoing(kMoving);

    if (fVel < 0)
    {
        setLineVelocity(-fVel);
        fDistance = -abs(fDistance);
    }
    else
    {
        fDistance = abs(fDistance);
    }

    CCPoint oLoc = getPosition();
    float fDegree = CCGameSprite::reviseRotation(getRotation());
    float fRedians = CC_DEGREES_TO_RADIANS(fDegree);

    CCFiniteTimeAction* pActMove0 = CCMoveBy::create(abs(fDistance / fVel), ccp(cos(fRedians) * fDistance, sin(-fRedians) * fDistance));
    CCFiniteTimeAction* pActMove1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actMoveEnd));
    CCAction* pActMove = CCSequence::create(pActMove0, pActMove1, NULL);

    stopMove();
    startDoing(kMoving);
    runAction(pActMove);
}

void CCNormalTankSprite::stopMove()
{
    if (isExploding() || isSuspended())
    {
        return;
    }
    endDoing(kTurning | kMoving);
    stopAllActions();
}

void CCNormalTankSprite::turnBy( float fDegree, float fVel )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    setTurningVelocity(fVel);

    if (fVel < 0)
    {
        setTurningVelocity(-fVel);
    }

    fDegree = CCGameSprite::reviseRotation(fDegree);
    CCFiniteTimeAction* pActTurn0 = CCRotateBy::create(abs(fDegree / fVel), fDegree);
    CCFiniteTimeAction* pActTurn1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actTurnEnd));
    CCAction* pActTurn = CCSequence::create(pActTurn0, pActTurn1, NULL);

    stopMove();
    startDoing(kTurning);
    runAction(pActTurn);
}

void CCNormalTankSprite::turnGunBy( float fDegree, float fVel )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    setTurningGunVelocity(fVel);

    if (fVel < 0)
    {
        setTurningGunVelocity(-fVel);
    }

    CCSprite* pGun = dynamic_cast<CCSprite*>(getChildByTag(0));

    CCFiniteTimeAction* pActTurnGun0 = CCRotateBy::create(abs(fDegree / fVel), fDegree);
    CCFiniteTimeAction* pActTurnGun1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalTankSprite::actTurnGunEnd));
    CCAction* pActTurnGun = CCSequence::create(pActTurnGun0, pActTurnGun1, NULL);

    pGun->stopAllActions();
    startDoing(kTurningGun);
    pGun->runAction(pActTurnGun);
}

void CCNormalTankSprite::suspend()
{
    if (isExploding())
    {
        return;
    }

    stop();
    startDoing(kSuspended);
}

void CCNormalTankSprite::resume()
{
    endDoing(kSuspended);
}

void CCNormalTankSprite::actExplodeEnd(CCNode* pNode)
{
    pNode->removeFromParentAndCleanup(true);
    
    endDoing(kExploding);
}

void CCNormalTankSprite::actMoveEnd( CCNode* pNode )
{
    endDoing(kMoving);
}

void CCNormalTankSprite::actTurnEnd( CCNode* pNode )
{
    endDoing(kTurning);
}

void CCNormalTankSprite::actTurnGunEnd( CCNode* pNode )
{
    endDoing(kTurningGun);
}

IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_GUN(CCSkillTankSprite);

void CCSkillTankSprite::skill1( float fPower, int iKey )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    setFirePower(fPower);

    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CBullet* pBulletLogic = CreateLogicObject<CBullet>();
    CCHomingMissileSprite* pBullet = CreateBulletSprite<CCHomingMissileSprite>("bullet/bullet2.png", pBulletLogic, this);
    //CCNormalBulletSprite* pBullet = CreateBulletSprite<CCNormalBulletSprite>("bullet/bullet2.png", pBulletLogic, this);

    CAttackData* pAtkData = CAttackData::create();
    pAtkData->setAttack(CAttackValue::kPhysical, getFirePower());

    CBuffSkill* pBuff = CHpChangeBuff::create(10, true, 0, 0.1, -0.9, false, 1);    
    pAtkData->addBuff(pBuff, 100);

    pAtkData = dynamic_cast<CUnit*>(getLogicBody())->attackAdv(pAtkData, NULL);
    pBullet->setAttackData(pAtkData);


    pBullet->fire(fPower, iKey, 1);
    //pBullet->runAction(CCAaaa::create());


    int iCount = pGm->getTankCount();
    for (int i = 0; i < iCount; i++)
    {
        CCTankSprite* pOtherTank = pGm->getTankByIndex(i);
        if (pOtherTank != this)
        {
            pOtherTank->triggerOnFire(pBullet);
        }
    }
}

void CCSkillTankSprite::skill2( float fDuration )
{
    if (isExploding() || isSuspended())
    {
        return;
    }

    CCGameManager* pGm = CCGameManager::sharedGameManager();
    pGm->playEffectSound("sound/DispelMagicTarget.wav");

    CCCoverAct* pActCover = CCCoverAct::create(fDuration);
    fDuration = pActCover->m_fDuration;
    runAction(pActCover);

    dynamic_cast<CUnit*>(getLogicBody())->addBuff(CHarmShieldBuff::create(fDuration, false, CExtraCoeff(0, 0), true));

}
