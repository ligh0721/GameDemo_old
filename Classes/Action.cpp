#include "CommInc.h"
#include "GameDisplay.h"
#include "Skill.h"
#include "Action.h"



CCCoverAct::~CCCoverAct()
{
    if (m_pSprite)
    {
        //m_pSprite->removeFromParentAndCleanup(true);
        m_pSprite = NULL;
    }
}

bool CCCoverAct::init( float fDuration )
{
    m_fDuration = fDuration;
    m_pSprite = NULL;

    return true;
}

void CCCoverAct::startWithTarget(CCNode *pTarget)
{
    CCAction::startWithTarget(pTarget);

    m_pSprite = CCSprite::createWithSpriteFrameName("bubble1.png");
    m_pTarget->addChild(m_pSprite, 10);

    m_pSprite->setPosition(m_pTarget->getAnchorPointInPoints());
    m_pSprite->setScale(0.6);
    m_pSprite->setOpacity(0);

    CCFiniteTimeAction* pActFade0 = CCFadeTo::create(0.3f, 80);
    CCFiniteTimeAction* pActFade1 = CCFadeTo::create(0.2, 30);
    CCFiniteTimeAction* pActFade2 = CCDelayTime::create(MAX(0, m_fDuration - 0.5f));
    CCFiniteTimeAction* pActFade3 = CCFadeTo::create(0.2, 0);
    CCFiniteTimeAction* pActFade4 = CCCallFuncN::create(this, callfuncN_selector(CCCoverAct::endEffect));
    CCAction* pActFade = CCSequence::create(pActFade0, pActFade1, pActFade2, pActFade3, pActFade4, NULL);
    m_pSprite->runAction(pActFade);

    CCFiniteTimeAction* pActScale0 = CCScaleTo::create(0.2f, 0.15f, 0.15f);
    CCFiniteTimeAction* pActScale1 = CCScaleTo::create(0.3, 0.2f, 0.2f);
    CCFiniteTimeAction* pActScale2 = CCDelayTime::create(MAX(0, m_fDuration - 0.5f));
    CCFiniteTimeAction* pActScale3 = CCScaleTo::create(0.2, 0.5f, 0.5f);
    CCAction* pActScale = CCSequence::create(pActScale0, pActScale1, pActScale2, pActScale3, NULL);
    m_pSprite->runAction(pActScale);
}

void CCCoverAct::endEffect( CCNode* pNode )
{
    if (m_pSprite)
    {
        m_pSprite->removeFromParentAndCleanup(true);
        m_pSprite = NULL;
    }
}

bool CCShakeAct::init( float fDuration, int iTimes, float fRange )
{
    CCActionInterval::initWithDuration(fDuration);
    m_iTimes = iTimes;
    m_fRange = fRange;
    return true;
}

void CCShakeAct::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    m_oLoc = m_pTarget->getPosition();
    float fDur = m_fDuration / m_iTimes / 4;
    CCFiniteTimeAction* pActShake = CCSequence::create(
        CCMoveTo::create(fDur, ccp(m_oLoc.x + m_fRange, m_oLoc.y)),
        CCMoveTo::create(fDur, ccp(m_oLoc.x, m_oLoc.y - m_fRange)),
        CCMoveTo::create(fDur, ccp(m_oLoc.x - m_fRange, m_oLoc.y)),
        CCMoveTo::create(fDur, ccp(m_oLoc.x, m_oLoc.y + m_fRange)),
        //CCMoveBy::create(m_fInterval, ccp(m_fRange, 0)),
        //CCMoveBy::create(m_fInterval, ccp(0, -m_fRange)),
        //CCMoveBy::create(m_fInterval, ccp(-m_fRange, 0)),
        //CCMoveBy::create(m_fInterval, ccp(0, m_fRange)),
        NULL);
    m_pTarget->runAction(CCSequence::create(CCRepeat::create(pActShake, m_iTimes), CCCallFuncN::create(this, callfuncN_selector(CCShakeAct::effectEnd)), NULL));
}

void CCShakeAct::update( float time )
{

}

void CCShakeAct::effectEnd( CCNode* pNode )
{
    if (m_pTarget)
    {
        m_pTarget->stopAllActions();
        m_pTarget->setPosition(m_oLoc);
    }

}

bool CCJumpByEx::init( const CCPoint& roPosition, const CCPoint& roDelta, const ccColor3B& roColor )
{
    m_oPosition = roPosition;
    m_oDelta = roDelta;
    m_oColor = roColor;
    return true;
}

void CCJumpByEx::startWithTarget( CCNode *pTarget )
{
    CCAction::startWithTarget(pTarget);
    
    CCRGBAProtocol* pRgbNode = dynamic_cast<CCRGBAProtocol*>(m_pTarget);
    CCAssert(pRgbNode, "m_pTarget must be a CCRGBAProtocol object");

    pRgbNode->setColor(m_oColor);
    //CCSize sz = pLabel->getContentSize();
    m_pTarget->setPosition(m_oPosition);
    CCFiniteTimeAction* pActFadeIn = CCFadeIn::create(0.05f);
    CCFiniteTimeAction* pActJumpTo = CCJumpBy::create(0.3f, m_oDelta, 50, 1);
    CCFiniteTimeAction* pActDelay = CCDelayTime::create(1.0f);
    CCFiniteTimeAction* pActFadeOut = CCFadeOut::create(2.0f);
    m_pTarget->stopAllActions();
    m_pTarget->runAction(CCSequence::create(pActFadeIn, pActJumpTo, pActDelay, pActFadeOut, NULL));
}

CCFlash::~CCFlash()
{
    if (m_pSprite)
    {
        //m_pSprite->removeFromParentAndCleanup(true);
        m_pSprite = NULL;
    }
}

bool CCFlash::init( float fDuration, CCSprite* pSprite, float fRotation )
{
    m_fDuration = fDuration;
    m_pSprite = pSprite;
    m_pSprite->retain();
    m_fRotation = fRotation;

    return true;
}

void CCFlash::startWithTarget( CCNode *pTarget )
{
    CCAction::startWithTarget(pTarget);

    m_pTarget->addChild(m_pSprite, 10);
    m_pSprite->release();

    m_pSprite->setPosition(m_pTarget->getAnchorPointInPoints());
    m_pSprite->setRotation(m_fRotation);
    m_pSprite->setScale(0.2);
    m_pSprite->setOpacity(0);

    CCFiniteTimeAction* pActFade0 = CCFadeTo::create(0.2, 128);
    CCFiniteTimeAction* pActFade2 = CCDelayTime::create(MAX(0, m_fDuration - 0.2));
    CCFiniteTimeAction* pActFade3 = CCFadeTo::create(0.5, 0);
    CCFiniteTimeAction* pActFade4 = CCCallFuncN::create(this, callfuncN_selector(CCFlash::endEffect));
    CCAction* pActFade = CCSequence::create(pActFade0, pActFade2, pActFade3, pActFade4, NULL);
    m_pSprite->runAction(pActFade);

    CCFiniteTimeAction* pActScale0 = CCScaleTo::create(0.05, 0.18f, 0.18f);
    CCFiniteTimeAction* pActScale1 = CCScaleTo::create(0.05, 0.21f, 0.21f);
    CCFiniteTimeAction* pActScale2 = CCDelayTime::create(MAX(0, m_fDuration - 0.1 + 0.4));
    CCFiniteTimeAction* pActScale3 = CCScaleTo::create(0.1, 0.22f, 0.22f);
    CCAction* pActScale = CCSequence::create(pActScale0, pActScale1, pActScale2, pActScale3, NULL);
    m_pSprite->runAction(pActScale);
}


void CCFlash::endEffect( CCNode* pNode )
{
    if (m_pSprite)
    {
        m_pSprite->removeFromParentAndCleanup(true);
        m_pSprite = NULL;
    }
}


bool CCSelected::init(CCNode* pNode)
{
    m_pNode = pNode;
    return true;
}

void CCSelected::startWithTarget( CCNode *pTarget )
{
    CCAction::startWithTarget(pTarget);
    
    pTarget->removeFromParentAndCleanup(true);
    m_pNode->addChild(pTarget);
    pTarget->setPosition(ccp(m_pNode->getContentSize().width / 2, m_pNode->getContentSize().height / 2));
    pTarget->setScale(1.2);
    //pSprite->setOpacity(0);

    /*
    CCFiniteTimeAction* pActFade0 = CCFadeTo::create(0.3f, 80);
    CCFiniteTimeAction* pActFade1 = CCFadeTo::create(0.2, 30);
    CCFiniteTimeAction* pActFade2 = CCDelayTime::create(MAX(0, m_fDuration - 0.5f));
    CCFiniteTimeAction* pActFade3 = CCFadeTo::create(0.2, 0);
    CCFiniteTimeAction* pActFade4 = CCCallFuncN::create(this, callfuncN_selector(CCCoverAct::endEffect));
    CCAction* pActFade = CCSequence::create(pActFade0, pActFade1, pActFade2, pActFade3, pActFade4, NULL);
    pSprite->runAction(pActFade);
    */

    CCFiniteTimeAction* pActScale0 = CCScaleTo::create(0.2f, 1.0f);
    //CCAction* pActScale = CCSequence::create(pActScale0, pActScale1, pActScale2, pActScale3, NULL);
    pTarget->runAction(pActScale0);
}

void CCSelected::endEffect( CCNode* pNode )
{

}

CCMoveToNode::~CCMoveToNode()
{
    CC_SAFE_RELEASE(m_pEndNode);
}

bool CCMoveToNode::init( float fDuration, CCNode* pNode, bool bFixRotation, float fMaxOffsetY, float fScaleY, float fOffsetYOfAnchor )
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_pEndNode=pNode;
        CC_SAFE_RETAIN(m_pEndNode);
        m_bFixRotation = bFixRotation;
        m_oEndPos=pNode->getPosition();
        //deltaPosition=endPosition;
        m_fMaxOffsetY = fMaxOffsetY;
        m_fScaleY = fScaleY;
        m_fOffsetYOfAnchor = fOffsetYOfAnchor;
        m_fMinSpeed = 0;
        return true;
    }
    return false;
}

void CCMoveToNode::startWithTarget( CCNode *pTarget )
{
    m_oStartPos=pTarget->getPosition();
    m_oEndPos=m_pEndNode->getPosition();
    m_oDeltaPos=ccpSub(m_oEndPos,m_oStartPos);
    m_fMinSpeed=sqrt(m_oDeltaPos.x*m_oDeltaPos.x+m_oDeltaPos.y*m_oDeltaPos.y/(m_fScaleY*m_fScaleY))/m_fDuration;
    CCActionInterval::startWithTarget(pTarget);

}

void CCMoveToNode::update( float time )
{
    if (m_pTarget)
    {
        m_oEndPos=m_pEndNode->getPosition();
        m_oEndPos.y += m_fOffsetYOfAnchor;
        m_oDeltaPos=ccpSub(m_oEndPos,m_oStartPos);
        setDuration(sqrt(m_oDeltaPos.x*m_oDeltaPos.x+m_oDeltaPos.y*m_oDeltaPos.y)/m_fMinSpeed);

        float fA = ccpDistance(m_oStartPos, m_oEndPos);
        
        float fX = time * fA - fA / 2;
        fA = -4 * m_fMaxOffsetY / (fA * fA);
        float fOffsetY = fA * fX * fX + m_fMaxOffsetY;
        float fOffsetR = atan(fA * fX);
        m_pTarget->setPosition(ccp(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time + fOffsetY));
        if (m_bFixRotation)
        {
            m_pTarget->setRotation(CC_RADIANS_TO_DEGREES((m_oStartPos.x > m_oEndPos.x ? fOffsetR : -fOffsetR) - ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))));
        }
        //m_pTarget->setRotation(CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))) + (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : -CC_RADIANS_TO_DEGREES(fOffsetR)));
        //CCLOG("%.2f, %.2f", CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))), (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : CC_RADIANS_TO_DEGREES(fOffsetR)));

         
    }
}

CCObject* CCMoveToNode::copyWithZone( CCZone* pZone )
{
    CCZone* pNewZone = NULL;
    CCMoveToNode* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCMoveToNode*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCMoveToNode();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->init(m_fDuration, m_pEndNode, m_bFixRotation, m_fOffsetYOfAnchor);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCLightning::~CCLightning()
{
    CC_SAFE_RELEASE(m_pStartNode);
    CC_SAFE_RELEASE(m_pEndNode);
}

bool CCLightning::init( CCAnimation* pAnimation, CCNode* pStartNode, CCNode* pEndNode, const CCPoint& roStartNodeOffsetOfAnchor /*= CCPointZero*/, float fEndNodeYOffsetOfAnchor /*= 0.0*/ )
{
    if (!CCAnimate::initWithAnimation(pAnimation))
    {
        return false;
    }
    CC_SAFE_RETAIN(pStartNode);
    m_pStartNode = pStartNode;
    CC_SAFE_RETAIN(pEndNode);
    m_pEndNode = pEndNode;
    m_oStartNodeOffsetOfAnchor = roStartNodeOffsetOfAnchor;
    m_fEndNodeOffsetOfAnchor = fEndNodeYOffsetOfAnchor;
    return true;
}

void CCLightning::startWithTarget( CCNode *pTarget )
{
    CCAnimate::startWithTarget(pTarget);
}

void CCLightning::update( float t )
{
    if (m_pTarget)
    {
        fixTargetPosition(m_pTarget);
    }

    CCAnimate::update(t);
}

void CCLightning::fixTargetPosition(CCNode* pTarget)
{
    CCPoint oStartPos = m_pStartNode->getPosition();
    CCPoint oEndPos = m_pEndNode->getPosition();
    CCSprite* pS = dynamic_cast<CCSprite*>(m_pStartNode);
    bool bFlipX;
    if (pS)
    {
        bFlipX = pS->isFlipX();
    }
    else
    {
        bFlipX = oStartPos.x > oEndPos.x;
    }
    oStartPos.x += bFlipX ? -m_oStartNodeOffsetOfAnchor.x : m_oStartNodeOffsetOfAnchor.x;
    oStartPos.y += m_oStartNodeOffsetOfAnchor.y;
    oEndPos.y += m_fEndNodeOffsetOfAnchor;
    float fR = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(oEndPos, oStartPos)));
    CCPoint oDelta = ccpSub(oEndPos, oStartPos);
    float fScale = sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) / pTarget->getContentSize().width;
    pTarget->setPosition(ccp((oStartPos.x + oEndPos.x) / 2, (oStartPos.y + oEndPos.y) / 2));
    pTarget->setRotation(fR);
    pTarget->setScaleX(fScale);
}

CCSequenceEx* CCSequenceEx::createWithTwoActions( CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo )
{
    CCSequenceEx *pSequence = new CCSequenceEx();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);
    pSequence->autorelease();

    return pSequence;
}

CCFiniteTimeAction* CCSequenceEx::getActionOne()
{
    return m_pActions[0];
}

CCFiniteTimeAction* CCSequenceEx::getActionTwo()
{
    return m_pActions[1];
}

bool CCMoveToEx::initWithDuration( float fDuration, const CCPoint& roPos, float fMaxOffsetY /*= 0.0*/, bool bFixRotation /*= true*/, float fScaleY /*= 1.0*/, float fYOffsetOfAnchor /*= 0.0*/ )
{
    m_fMaxOffsetY = fMaxOffsetY;
    m_fScaleY = fScaleY;
    m_fYOffsetOfAnchor = fYOffsetOfAnchor;
    m_bFixRotation = bFixRotation;
    return CCMoveTo::initWithDuration(fDuration, roPos);
}

void CCMoveToEx::update( float time )
{
    if (m_pTarget)
    {
        float fA = ccpDistance(m_startPosition, m_endPosition);

        float fX = time * fA - fA / 2;
        fA = -4 * m_fMaxOffsetY / (fA * fA);
        float fOffsetY = fA * fX * fX + m_fMaxOffsetY;
        float fOffsetR = atan(fA * fX);
        m_pTarget->setPosition(ccp(m_startPosition.x + m_delta.x * time, m_startPosition.y + m_delta.y * time + fOffsetY));
        if (!m_bFixRotation)
        {
            m_pTarget->setRotation(CC_RADIANS_TO_DEGREES((m_startPosition.x > m_endPosition.x ? fOffsetR : -fOffsetR) - ccpToAngle(ccpSub(m_endPosition, m_startPosition))));
        }
    }
}

CCObject* CCMoveToEx::copyWithZone( CCZone* pZone )
{
    CCZone* pNewZone = NULL;
    CCMoveToEx* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCMoveToEx*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCMoveToEx();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCMoveTo::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_endPosition, m_fMaxOffsetY, m_bFixRotation, m_fScaleY, m_fYOffsetOfAnchor);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

const float CCDelayRelease::CONST_EX_DURATION = 1.0;

bool CCDelayRelease::initWithDuration( float fDelay )
{
    CCSequence::initWithTwoActions(CCSequence::createWithTwoActions(CCDelayTime::create(fDelay), CCFadeOut::create(CONST_EX_DURATION)), CCCallFuncN::create(this, callfuncN_selector(CCDelayRelease::onActEnd)));
    return true;
}

void CCDelayRelease::onActEnd( CCNode* pNode )
{
    pNode->removeFromParentAndCleanup(true);
}

bool CCReleaseAfter::initWithAction( CCFiniteTimeAction* fAct )
{
    return initWithTwoActions(fAct, CCCallFuncN::create(this, callfuncN_selector(CCReleaseAfter::onActEnd)));
}

void CCReleaseAfter::onActEnd( CCNode* pNode )
{
    pNode->removeFromParentAndCleanup(true);
}
