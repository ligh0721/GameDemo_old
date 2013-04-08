#include "CommInc.h"
#include "GameDisplay.h"
#include "GameLogic.h"
#include "Action.h"
#include "TankSprite.h"
#include "Unit.h"
#include "BattleGroundScene.h"
#include "BulletSprite.h"


CCBulletSprite::~CCBulletSprite()
{

}

bool CCBulletSprite::initWithSpriteFrameNameAndLogic( const char *pszSpriteFrameName, CBullet* pBullet )
{
    initWithSpriteFrameName(pszSpriteFrameName);
    setLogicBody(pBullet);
    dynamic_cast<CDisplayInfoInterface*>(getLogicBody())->setDisplayBody(this);

    return true;
}

CCTankSprite* CCBulletSprite::getOwner() const
{
    return CCGameManager::sharedGameManager()->getTankByKey(dynamic_cast<CBullet*>(getLogicBody())->getOwnerKey());
}

bool CCBulletSprite::collisionDetection()
{
    CCTankSprite* pOwner = getOwner();
    if (!pOwner)
    {
        explode();
        return true;
    }
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCPoint oPb;
    CCPoint oPt;
    int iCount = pGm->getTankCount();
    for (int i = 0; i < iCount; i++)
    {
        CCTankSprite* pOtherTank = pGm->getTankByIndex(i);
        if (pOtherTank == pOwner || pOtherTank->isExploding())
        {
            continue;
        }
        oPb = getPosition();
        oPt = pOtherTank->getPosition();
        float fDis = ccpDistance(oPb, oPt);
        //float fMinDis = pOtherTank->boundingBox().size.width / 2 + boundingBox().size.width / 2;
        //if (fDis < fMinDis)
        if (pOtherTank->boundingBox().intersectsRect(boundingBox()))
        {
            // 将来需要加一个onCollision()，并将下列代码移至其中
            CTank* pTankL = dynamic_cast<CTank*>(pOtherTank->getLogicBody());
            CAttackData* pAtkData = getAttackData();
            pAtkData->setAngle(-CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(oPb, oPt))));
            pTankL->damagedAdv(pAtkData, dynamic_cast<CUnit*>(pOwner->getLogicBody()));
            setAttackData(NULL);
            
            explode();
            return true;
        }
    }
    iCount = pGm->getBulletCount();
    for (int i = 0; i < iCount; i++)
    {
        CCBulletSprite* pOtherBullet = pGm->getBulletByIndex(i);
        if (pOtherBullet == this || pOtherBullet->getOwner() == pOwner || pOtherBullet->isExploding())
        {
            continue;
        }
        oPb = getPosition();
        oPt = pOtherBullet->getPosition();
        float fDis = ccpDistance(oPb, oPt);
        float fMinDis = pOtherBullet->boundingBox().size.width / 2 + boundingBox().size.width / 2;
        if (fDis < fMinDis)
        {
            explode();
            pOtherBullet->explode();
            return true;
        }
    }
    return false;
}

CCNormalBulletSprite::CCNormalBulletSprite()
    : m_fRange(0)
{
}

CCNormalBulletSprite::~CCNormalBulletSprite()
{
}

IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(CCNormalBulletSprite);

void CCNormalBulletSprite::update( float dt )
{
    if (isExploding())
    {
        return;
    }
    collisionDetection();
}

void CCNormalBulletSprite::actMoveEnd( CCNode* pNode )
{
    explode();
}

void CCNormalBulletSprite::actExplodeEnd( CCNode* pNode )
{
    endDoing(kExploding);
    pNode->removeFromParentAndCleanup(true);
}

void CCNormalBulletSprite::explode()
{
    if (isExploding())
    {
        return;
    }

    CCTankSprite* pTank = getOwner();
    //CCAssert(pTank, "the bullet havenot a owner??");
    if (pTank)
    {
        CTank* pTankL = dynamic_cast<CTank*>(pTank->getLogicBody());
        pTankL->onDestroyProjectile(this);
    }
    

    //getGameManager()->playEffectSound("sound/GlueScreenMeteorHit1.wav");
    
    CCFiniteTimeAction* pActExplode0 = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("explode"));
    CCFiniteTimeAction* pActExplode1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalBulletSprite::actExplodeEnd));
    CCAction* pActExplode = CCSequence::create(pActExplode0, pActExplode1, NULL);
    
    stopAllActions();
    startDoing(kExploding);
    runAction(pActExplode);
}

bool CCNormalBulletSprite::isExploding() const
{
    return isDoingOr(kExploding);
}

void CCNormalBulletSprite::fire()
{
    float fRedians = -CC_DEGREES_TO_RADIANS(getRotation());
    CCFiniteTimeAction* pActMove0 = CCMoveBy::create(m_fRange / getLineVelocity(), ccp(cos(fRedians) * m_fRange, sin(fRedians) * m_fRange));
    CCFiniteTimeAction* pActMove1 = CCCallFuncN::create(this, callfuncN_selector(CCNormalBulletSprite::actMoveEnd));
    CCAction* pActMove = CCSequence::create(pActMove0, pActMove1, NULL);
    runAction(pActMove);
}

void CCNormalBulletSprite::fire( float fPower, float fVel, float fRange, float fScaleX, float fScaleY )
{
    dynamic_cast<CBullet*>(getLogicBody())->setPower(fPower);
    setLineVelocity(fVel);
    setRange(fRange);
    setScaleX(fScaleX);
    setScaleY(fScaleY);
    
    fire();
}


CCHomingMissileSprite::CCHomingMissileSprite()
    : m_pPhyBody(NULL)
    , m_iTargetKey(0)
    , m_fForceDuration(0.0)
    , m_fSumDt(0.0)
    , m_pEmitter(NULL)
{

}

CCHomingMissileSprite::~CCHomingMissileSprite()
{
    CCGameManager::sharedGameManager()->m_pPhyWorld->DestroyBody(m_pPhyBody);
}

IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(CCHomingMissileSprite);

void CCHomingMissileSprite::updateBodyByPhyBody()
{
    b2Vec2 oPos = m_pPhyBody->GetPosition();
    setPosition(ccp(oPos.x * PTM_RATIO, oPos.y * PTM_RATIO));
    setRotation(-CC_RADIANS_TO_DEGREES(m_pPhyBody->GetAngle()));
}
#if 0
bool CCHomingMissileSprite::isDirty( void )
{
    return true;
}

cocos2d::CCAffineTransform CCHomingMissileSprite::nodeToParentTransform( void )
{
    b2Vec2 pos  = m_pPhyBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if (isIgnoreAnchorPointForPosition()) {
        x += m_tAnchorPointInPoints.x;
        y += m_tAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pPhyBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if(! m_tAnchorPointInPoints.equals(CCPointZero)){
        x += c*-m_tAnchorPointInPoints.x + -s*-m_tAnchorPointInPoints.y;
        y += s*-m_tAnchorPointInPoints.x + c*-m_tAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_tTransform = CCAffineTransformMake(c,  s,
        -s,    c,
        x,    y);

    setPosition(ccp(x, y));
    setRotation(-CC_RADIANS_TO_DEGREES(-radians));
    return m_tTransform;
}
#endif
void CCHomingMissileSprite::update( float dt )
{
    updateBodyByPhyBody();
    m_fSumDt += dt;
    if (isExploding())
    {
        return;
    }

    if (collisionDetection())
    {
        return ;
    }
    
    CCTankSprite* pOwner = getOwner();
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCRect oRt = boundingBox();
    CBullet* pBullet = dynamic_cast<CBullet*>(getLogicBody());
    if (m_pEmitter)
    {
        //float fR = 180 - getRotation();
        //m_pEmitter->setGravity(ccp(500 * cos(fR), 500 * sin(-fR)));
        m_pEmitter->setPosition(pGm->m_pBattleGroundLayer->convertToNodeSpace(convertToWorldSpace(ccp(0, getContentSize().height / 2))));
        //m_pEmitter->setAngle(fR);
        m_pEmitter->setRotation(getRotation());
    }

    CCTankSprite* pTarget = pGm->getTankByKey(getTargetKey());
    if (!pTarget || pTarget->isExploding())
    {
        int iCount = pGm->getTankCount();
        for (int i = 0; i < iCount; i++)
        {
            pTarget = pGm->getTankByIndex(i);
            if (pTarget != pOwner)
            {
                break;
            }
            else
            {
                pTarget = NULL;
            }
        }
    }

    float fRedians = m_pPhyBody->GetAngle();
    m_oVecForce.Set(cos(fRedians) * pBullet->getPower(), sin(fRedians) * pBullet->getPower());

    if (pTarget)
    {
        if (m_fSumDt > m_fForceDuration)
        {
            b2Vec2 oVec(pTarget->getPosition().x / PTM_RATIO, pTarget->getPosition().y / PTM_RATIO);
            b2Vec2 oFp = getHeadPoint();
            CCPoint oGl = pTarget->getPosition();
            float fDelta = ccpToAngle(ccpSub(oGl, ccp(oFp.x * PTM_RATIO, oFp.y * PTM_RATIO)));
            m_pPhyBody->ApplyForce(b2Vec2(30 * pBullet->getPower() * cos(fDelta), 30 * pBullet->getPower() * sin(fDelta)), oFp);
            //m_oVecForce *= 10;
        }
        else
        {
            m_oVecForce *= 40;
            m_pPhyBody->ApplyForce(m_oVecForce, getTailPoint());
        }
    }
    else
    {
        explode();
    }

}

void CCHomingMissileSprite::explode()
{
    if (isExploding())
    {
        return;
    }

    CCTankSprite* pTank = getOwner();
    if (pTank)
    {
        dynamic_cast<CUnit*>(pTank->getLogicBody())->onDestroyProjectile(this);// unsafe use key insteaded
    }

    CCGameManager* pGm = CCGameManager::sharedGameManager();
    pGm->playEffectSound("sound/Fire_Napalm.wav");

    pGm->m_pBattleGroundLayer->runAction(CCShakeAct::create(0.7, 9, 10));

    if (m_pEmitter)
    {
        m_pEmitter->stopSystem();
    }
    
    CCFiniteTimeAction* pActExplode0 = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("explode"));
    CCFiniteTimeAction* pActExplode1 = CCCallFuncN::create(this, callfuncN_selector(CCHomingMissileSprite::actExplodeEnd));
    CCAction* pActExplode = CCSequence::create(pActExplode0, pActExplode1, NULL);

    stopAllActions();
    startDoing(kExploding);
    runAction(pActExplode);
}

bool CCHomingMissileSprite::isExploding() const
{
    return isDoingOr(kExploding);
}

void CCHomingMissileSprite::actExplodeEnd( CCNode* pNode )
{
    endDoing(kExploding);
    pNode->removeFromParentAndCleanup(true);
    if (m_pEmitter)
    {
        m_pEmitter->removeFromParentAndCleanup(true);
        //CCLOG("%p", m_pEmitter);
    }
}

void CCHomingMissileSprite::fire()
{
    CBullet* pBullet = dynamic_cast<CBullet*>(getLogicBody());
    m_pEmitter = CCParticleSun::create();

    CCGameManager* pGm = CCGameManager::sharedGameManager();
    if (m_pEmitter)
    {
        m_pEmitter->setAutoRemoveOnFinish(false);
        m_pEmitter->setEmissionRate(350);
        m_pEmitter->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("particle/fire.png"));
        m_pEmitter->setPositionType(kCCPositionTypeFree);
        pGm->m_pParticleBatch->addChild(m_pEmitter);
    }
    

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(getPositionX() / PTM_RATIO, getPositionY() / PTM_RATIO);
    m_pPhyBody = pGm->m_pPhyWorld->CreateBody(&bodyDef);
    m_pPhyBody->SetBullet(true);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(getContentSize().width / PTM_RATIO / 2, getContentSize().height / PTM_RATIO / 2);//These are mid points for our 1m box

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;    
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    m_pPhyBody->CreateFixture(&fixtureDef);
    m_pPhyBody->SetLinearDamping(0.5);
    m_pPhyBody->SetAngularDamping(10);
    m_pPhyBody->SetTransform(b2Vec2(getPositionX() / PTM_RATIO, getPositionY() / PTM_RATIO), CC_DEGREES_TO_RADIANS(-getRotation()));

}

void CCHomingMissileSprite::fire( float fPower, int iTargetKey, float fInitDuration )
{
    dynamic_cast<CBullet*>(getLogicBody())->setPower(fPower);
    setTargetKey(iTargetKey);
    setForceDuration(fInitDuration);
    fire();
}

b2Vec2 CCHomingMissileSprite::getHeadPoint()
{
    b2Vec2 oCt = m_pPhyBody->GetWorldCenter();
    float fDelta = boundingBox().size.width / PTM_RATIO / 2;
    oCt.x += fDelta * cos(m_pPhyBody->GetAngle());
    oCt.y += fDelta * sin(m_pPhyBody->GetAngle());
    return oCt;
}

b2Vec2 CCHomingMissileSprite::getTailPoint()
{
    b2Vec2 oCt = m_pPhyBody->GetWorldCenter();
    float fDelta = boundingBox().size.width / PTM_RATIO / 2;
    oCt.x -= fDelta * cos(m_pPhyBody->GetAngle());
    oCt.y -= fDelta * sin(m_pPhyBody->GetAngle());
    return oCt;
}

float CCHomingMissileSprite::getLineVelocity() const
{
    b2Vec2 oVel = m_pPhyBody->GetLinearVelocity();
    return sqrt(oVel.x * oVel.x + oVel.y + oVel.y);
}

float CCHomingMissileSprite::getAngularVelocity() const
{
    return m_pPhyBody->GetAngularVelocity();
}
